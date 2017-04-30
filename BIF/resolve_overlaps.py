#!/usr/bin/env python3
import sys
from collections import defaultdict
from operator import attrgetter as get
from datetime import date


class Sequence:
    """Sequence is a superset of Features.

    Sequence encapsulates set of Features. Each sequence has a precalculated
    'score' and is marked by 'start' and 'end'. Features listed in a Sequence
    are accessible via 'nodes' member.
    """

    def __init__(self, score, start, end, nodes):
        """Create a Sequence."""
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
    """Feature, an area building sequences without overlaps.

    Each Feature is symbolized by a line in the GFF file. A Feature has a
    'start' point, 'end' point and is marked by a 'score'. Original entry is
    saved in 'line' property.
    """

    def __init__(self, line, start, end, score):
        """Create a Feature."""
        self.line = line
        self.start = start
        self.end = end
        self.score = score

        # Prepare connecting with other Features - via set of successors
        self.succ = set()
        self.has_succ = False

    def make_succ(self, node):
        """Add a successor Feature to internal set.

        If specific conditions are met, add the 'node' into internal 'succ'
        list. In case the 'node' is really added, provide such information
        also by the 'node' itself (via 'has_succ' attribute)
        """
        # At first, filter out nodes which doesn't fit
        if not node.start > self.end:
            # If the node doesn't start after this Feature ends, skip it
            return
        if self.succ and node.start > min(self.succ, key=get('end')).end:
            # If the node starts after one of successors ends, skip as well
            return
        if any(node.score < n.score for n in self.succ if node.end >= n.end):
            # If node ends after any current successor and has worse score,
            # skip it (we're able to create shorter and better Sequence)
            return
        # Node is a true successor

        # Mark the node
        node.has_succ = True

        # If the node is ending before any of current successor, check if it's
        # score is higher (remove weaker successors)
        self.succ = set(
            s
            for s in self.succ
            if s.end < node.end or node.score <= s.score
        )

        # Add the node to our set of successors
        self.succ.add(node)


def get_line(input_file):
    """Feature generator parsing input file."""
    with open(input_file, 'r') as f:
        for line in f:
            # Skip empty or commented lines
            if len(line) <= 1 or line[0] == '#':
                continue

            ln = line.split()
            # Create an unique ID defining the strand type
            fid = "{0} {1} {2}".format(ln[0], ln[2], ln[6])

            # Get me the line!
            yield fid, Feature(line, int(ln[3]), int(ln[4]), int(ln[5]))


def find_routes(input_file):
    """Create graph from Features, build Sequences and find the best one.

    View the Overlapping as a graph. Take each Feature as a node and connect it
    forward - with its successors. Then iterate through all starting nodes
    (they are not successors of any other node). For each such node, take it's
    successors and add this sequence to scope. Repeat with all these Sequences
    and find the best!
    """
    graph = defaultdict(set)
    best_routes = dict()

    # Init graph nodes (Features)
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
        start_points = iter(n for n in nodes if not n.has_succ)

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

                routes.update(
                    Sequence(r.score+n.score, r.start, n.end, (*r.nodes, n))
                    for n in r.nodes[-1].succ
                )
        # Add t_best to globally tracked best_routes
        best_routes[t] = t_best
        print()

    return best_routes


def print_gff(data_dict):
    """Print results to stdout in GFF format.

    Loop through Features in Sequences and reuse the lines from input file.
    GFF specification:
    https://github.com/The-Sequence-Ontology/Specifications/blob/master/gff3.md
    """
    # Header
    print('##gff-version 3')
    print('##date %s' % date.today())

    # Meta information to verify results
    for seq in data_dict.keys():
        print("# Best score of '{0}': {1}".format(seq, data_dict[seq].score))

    # Dump lines
    for _, series in data_dict.items():
        [print(node.line, end="") for node in series.nodes]


if __name__ == '__main__':
    try:
        best_sequences = find_routes(sys.argv[1])
        print_gff(best_sequences)
    except IndexError:
        print('Missing input file')
