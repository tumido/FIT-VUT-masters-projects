#!/usr/bin/env python3
import sys
from collections import defaultdict
from datetime import date
from operator import attrgetter

__author__ = 'xcoufa09'


class Sequence:
    """Sequence is a superset of Features.

    Sequence encapsulates set of Features. Each sequence has a precalculated
    'score' and is marked by 'end'. Features listed in a Sequence
    are accessible via 'nodes' member.
    """

    def __init__(self, score, end, nodes):
        """Create a Sequence."""
        self.score = score
        self.end = end
        self.nodes = nodes
        self.child_end = None
        self.children = set()


class Feature:
    """Feature, an area building sequences without overlaps.

    Each Feature is symbolized by a line in the GFF file. A Feature has a
    'start' point, 'end' point and is marked by a 'score'. Original entry is
    marked by 'line' property.
    """

    def __init__(self, fid, line_no, line, start, end, score):
        """Create a Feature."""
        self.fid = fid
        self.line_no = line_no
        self.line = line
        self.start = start
        self.end = end
        self.score = score


def yield_line(input_file, sort=True):
    """Feature generator parsing input file."""
    data = list()
    with open(input_file, 'r') as f:
        # Start counting lines
        for line_no, line in enumerate(f, start=1):
            # if line_no > 20000:
            #     break
            # Skip empty or commented lines
            if len(line) <= 1 or line[0] == '#':
                continue

            l = line.split()
            # Create an unique ID defining the strand type
            fid = "{0} {1} {2}".format(l[0], l[2], l[6])

            # Get me the line!
            f = Feature(fid, line_no, line, int(l[3]), int(l[4]), int(l[5]))
            if sort:
                data.append(f)
            else:
                yield f

        if not sort:
            raise StopIteration()

        data = sorted(data, key=attrgetter('start'))
        for f in data:
            yield f


def print_seqv(s, in_file):
    """Print Features in a set."""
    # pylint: disable=expression-not-assigned
    [print(n.line, end="") for n in s.nodes]


def update_route(add, remove, all_routes, route, feature):
    """Find routes which are elongated by or removed because of 'feature'.

    From 'all_routes' remove worse routes than current 'route'. Then find if
    the 'route' should be elongated. If so, inject it into 'add' set and update
    'route' properties.
    """
    if route in remove:
        return

    # Remove obviously worse Sequences
    if any(k.end <= route.end and route.score < k.score for k in all_routes):
        remove.add(route)
        return

    # Fragments starts after a children of 'r' Sequence
    if route.child_end and feature.start > route.child_end:
        remove.add(route)
        return

    # Feature is overlapping
    if feature.start <= route.end:
        return

    # Candidate Feature successor for a Sequence
    seqv = Sequence(route.score+feature.score, feature.end,
                    (*route.nodes, feature))

    # Any of newly created is better on first sight
    if any(a.end <= seqv.end and seqv.score < a.score for a in add):
        return

    # Any of sibling Sequences (derived form the same sequence) is
    # obviously better
    if any(c.end <= seqv.end and seqv.score < c.score for c in route.children):
        return

    # Candidate passed, add the sequence to 'to_add' and update parent
    add.append(seqv)
    if route.child_end:
        route.child_end = min(route.child_end, seqv.end)
    else:
        route.child_end = seqv.end
    route.children.add(seqv)


def find_routes(input_file, generator):
    """Find the best routes."""
    # Loop each strand type separately
    longest = dict()
    closest = dict()
    routes = defaultdict(list)
    best_total = defaultdict(int)

    # Print GFF header
    print('##gff-version 3')
    print('##date %s' % date.today())

    # Loop input file lines
    while True:
        try:
            f = next(generator)
            t = f.fid
        except StopIteration:
            break

        # If the loaded Feature ends after any of current Sequences,
        # get the partial best route and start over
        if t in longest.keys() and f.start > longest[t]:
            # Get best and count score
            best = max(routes[t], key=attrgetter('score'))
            best_total[t] += best.score

            # Print The Sequence
            print(" "*100, end='\r')
            print("# partial score for '{0}': {1}".format(t, best_total[t]))
            print_seqv(best, input_file)

            # Clear the stack
            routes[t].clear()
            closest.pop(t, None)
            longest.pop(t, None)

        # Update boundaries
        if t in longest.keys():
            longest[t] = max(longest[t], f.end)
            closest[t] = min(closest[t], f.end)
        else:
            longest[t] = closest[t] = f.end

        # If the Feature can't be added to any Sequence, add it as a new
        # Sequence
        if closest[t] > f.start:
            routes[t].append(Sequence(f.score, f.end, (f,)))
            continue

        # Loop and elongate (or remove) current lines
        to_add = list()
        to_remove = set()
        for r in routes[t]:
            update_route(to_add, to_remove, routes[t], r, f)

        routes[t].extend(to_add)
        # pylint: disable=expression-not-assigned
        [routes[t].remove(x) for x in to_remove if x in routes[t]]

        print("routes: {0:10}, line: {1:10}".format(len(routes[t]), f.line_no),
              end="\r")

    # File is done, print the best route in the last block
    for t, v in routes.items():
        best = max(routes[t], key=attrgetter('score'))
        best_total[t] += best.score
        print_seqv(best, input_file)

    # Print stats
    for t, v in best_total.items():
        print("# {0}: {1}".format(t, v))


if __name__ == '__main__':
    try:
        is_sorted = sys.argv[1] != '-s'
        gen = yield_line(sys.argv[-1], sort=is_sorted)
        find_routes(sys.argv[-1], gen)
    except IndexError:
        print('Missing input file')
