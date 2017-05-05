#!/usr/bin/env python3
import sys
from collections import defaultdict
from datetime import date
from operator import attrgetter
from bisect import bisect_left

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

    def __add__(self, feature):
        """Append Feature to the Sequence."""
        return Sequence(self.score + feature.score,
                        feature.end, (*self.nodes, feature))

    def __gt__(self, other):
        """Compare Sequences by score."""
        return self.score > other.score


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
        # self.pred = None


def yield_line(input_file, sort=True):
    """Feature generator parsing input file."""
    data = list()
    # Used algorithm require sorted file
    if sort:
        print("# File will be sorted")

    with open(input_file, 'r') as f:
        # Start counting lines
        for line_no, line in enumerate(f, start=1):
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

        # Sort data and yield it
        data = sorted(data, key=attrgetter('start'))
        for f in data:
            yield f


def find_routes(input_file, generator):
    """Find the best non-overlapping Sequence in a file.

    Divide file into blocks and find the best Sequences for each.
    """
    # Print GFF header
    print('##gff-version 3')
    print('##date %s' % date.today())

    # Remember parsed features, top score for each type and the most far end
    features = defaultdict(list)
    top_score = defaultdict(int)
    most_far = defaultdict(int)

    # Loop Features
    for f in generator:
        # Divide file into blocks is possible thanks to file being sorted by
        # 'start' of each Feature
        # When feature starts after all on previously discovered, the best
        # partial Sequence can be located here no matter what follows
        if f.fid in most_far.keys() and f.start > most_far[f.fid]:
            top_score[f.fid] += get_partial(features[f.fid])
            features[f.fid].clear()

        # Update the latest seen 'end'
        if f.fid in most_far.keys():
            most_far[f.fid] = max(f.end, most_far[f.fid])
        else:
            most_far[f.fid] = f.end

        # Build list of features
        features[f.fid].append(f)

    # File parsed completely, find routes in the last block
    for fid in features.keys():
        top_score[fid] += get_partial(features[fid])

    # Print overall statistics
    for t, score in top_score.items():
        print("# Type '{0}' scored: {1:10}".format(t, score))


def get_partial(routes):
    """Find the Sequence with best score.

    Loop routes and build up Sequences. Find the best one by score.
    """
    sequences = defaultdict(Sequence)
    # To successfully find the Sequences we need to get the ones which ends
    # before our Feature - Sort by end of Feature
    routes.sort(key=attrgetter('end'))

    # Index starts and ends
    start = [f.start for f in routes]
    end = [f.end for f in routes]

    # Initial conditions
    sequences[0] = Sequence(0, 0, set())
    sequences[1] = Sequence(routes[0].score, routes[0].end, (routes[0],))

    # Find best sequences
    for i in range(2, len(routes)+1):
        # Find best suitable for elongating
        new_seq = sequences[bisect_left(end, start[i-1])] + routes[i-1]
        # Remember the best one
        sequences[i] = max(sequences[i-1], new_seq)

    # Print the best route
    for n in sequences[len(routes)].nodes:
        print(n.line, end='')

    return sequences[len(routes)].score


if __name__ == '__main__':
    try:
        # Enable optimized parsing - file is pre-sorted
        is_sorted = sys.argv[1] != '-s'

        # The reason for using sorted data is, we can walk the file and divide
        # it into separate small blocks (in find_routes)
        gen = yield_line(sys.argv[-1], sort=is_sorted)

        # Find the best Sequences
        find_routes(sys.argv[-1], gen)
    except IndexError:
        print('Missing input file')
