#!/usr/bin/env python3
import sys
from collections import defaultdict
from operator import attrgetter as get
from datetime import date


class Sequence:
    def __init__(self, score, start, end, nodes):
        self.score = score
        self.start = start
        self.end = end
        self.nodes = nodes

    def __lshift__(self, other):
        """Using this operator to compare scores.

        Sequence is << than other if it's score is lower.
        """
        return self.score < other.score

    def __lt__(self, other):
        """Sequence is smaller if it is shorter.

        Shorter means it starts later and ends before the other Sequence
        """
        return other.start <= self.start and self.end <= other.end


class Feature:
    def __init__(self, line, start, end, score):
        self.line = line
        self.start = start
        self.end = end
        self.score = score
        self.succ = set()
        self.has_succ = False
    def make_succ(self, node):
        # Add node as successor if it meets requirements
        if not node.start > self.end:
            # If the node doesn't start after this Feature ends, skip it
            return
        if self.succ and node.start > min(self.succ, key=get('end')).end:
            # If the node starts after one of successors ends, skip as well
            return
        if any(node.score < n.score for n in self.succ if node.end >= n.end):
            # If node ends after any current successor and has worse score,
            # skip it
            return
        # Node is a successor, we're interested in

        # Mark the node - it is a successor of something
        node.has_succ = True

        # If the node is ending before any of current successor, check if it's
        # score is higher
        self.succ = set(s for s in self.succ if s.end < node.end or node.score <= s.score)

        # Node proved as a useful successor, adding it to set
        self.succ.add(node)


def get_line(input_file):
    """Feature generator parsing input file."""
    with open(input_file, 'r') as f:
        i = 0
        for line in f:
            i+=1
            if i >= 1500:
                break
            # Skip empty or commented lines
            if len(line) <= 1 or line[0] == '#':
                continue

            l = line.split()
            # Create an unique ID
            fid = "{0} {1} {2}".format(l[0], l[2], l[6])

            # Get me the line!
            yield fid, Feature(line, int(l[3]), int(l[4]), int(l[5]))


def find_routes(input_file):
    graph = defaultdict(set)
    best_routes = dict()

    # Init Graph nodes (Features)
    [graph[t].add(f) for t, f in get_line(input_file)]

    # Loop each strand type separately
    for t, nodes in graph.items():
        # Remember the top score route
        t_best = None

        # Place the node into the graph
        for node in nodes:
            # Find the valid successors
            [node.make_succ(n) for n in nodes]
            # Keep track of the best score
            if not t_best or node.score > t_best.score:
                t_best = Sequence(node.score, node.start, node.end, (node,))

        # Find proper starting places
        start_points = set(n for n in nodes if not n.has_succ)

        # Start the search from nodes with no predecessors
        for p in start_points:
            # Generate initial routes
            routes = set(
                Sequence(p.score+s.score, p.start, s.end, (p, s))
                for s in p.succ
            )

            # Find the best route
            while routes:
                r = routes.pop()
                print(
                    "All possible routes: {0:10}, Top score: {1:10}".format(
                        len(routes), t_best.score),
                    end='\r'
                )

                # Skip the route if current best is already shorter but better
                if t_best < r and r << t_best:
                    continue

                # Remove further routes which would be worse than this one
                useless_routes = set(k for k in routes if r < k and k << r)
                routes.difference_update(useless_routes)

                # This node has no further successors, just check the score and
                # continue
                if not r.nodes[-1].succ:
                    if r.score > t_best.score:
                        t_best = r
                    continue

                routes.update(set(
                    Sequence(r.score+n.score, r.start, n.end, (*r.nodes, n))
                    for n in r.nodes[-1].succ
                ))
        # Add t_best to globally tracked best_routes
        best_routes[t] = t_best
        print()

    return best_routes


def print_gff(data_dict):
    print('##gff-version 3')
    print('##date %s' % date.today())

    for seq in data_dict.keys():
        print("# Best score of '{0}': {1}".format(seq, data_dict[seq].score))

    for _, series in data_dict.items():
        for node in series.nodes:
            print(node.line, end="")


if __name__ == '__main__':
    try:
        best_routes = find_routes(sys.argv[1])
        print_gff(best_routes)

    except IndexError:
        print('Missing input file')
