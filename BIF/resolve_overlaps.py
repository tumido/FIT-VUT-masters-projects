#!/usr/bin/env python3
import sys
from collections import defaultdict
from datetime import date
from linecache import getline
from operator import attrgetter
from itertools import islice

__author__ = 'xcoufa09'


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
    marked by 'line' property.
    """

    def __init__(self, line, start, end, score):
        """Create a Feature."""
        self.line = line  # Line number pointing to the original file
        self.start = start
        self.end = end
        self.score = score

        # Prepare connecting with other Features - via set of successors
        self.succ = set()
        self.has_pred = False  # Flag the node if it has predecessors
        self._succ_closest_end = None
        self.all_successors_generated = False

    def gen_successors(self, candidates, is_in_list=True):
        """Wrapper generating all successors.

        If successors are not already generated loop through the graph and find
        all viable nodes. Try to make them successors. When a node, which can
        be a successor of any of 'self.succ', is found, generation is complete.
        """
        if self.all_successors_generated:
            # Successors already generated
            return

        # Slice the 'candidates' for faster iterations
        idx = candidates.index(self) if is_in_list else 0
        for i in islice(candidates, idx, None):
            if self.make_succ(i):
                # Generation complete
                break

        self.all_successors_generated = True

    def make_succ(self, node, sorted_file=False):
        """Add a successor Feature to internal set.

        If specific conditions are met, add the 'node' into internal 'succ'
        list. In case the 'node' is really added, provide such information
        also by the 'node' itself (via 'has_pred' attribute)

        :return: True if 'node' is too far to be a successor (end reached)
        """
        # At first, filter out nodes which doesn't fit
        if not node.start > self.end:
            # If the node doesn't start after this Feature ends, skip it
            return False

        # Node can be a successor, mark it
        node.has_pred = True

        # if self.succ and node.start > min(self.succ, key=get('end')).end:
        if self._succ_closest_end and self._succ_closest_end < node.start:
            # If the node starts after one of successors ends, we reached
            # the end
            return True
        if any(node.score < n.score for n in self.succ if node.end >= n.end):
            # If node ends after any current successor and has worse score,
            # skip it (we're able to create shorter and better Sequence)
            return False
        # Node is a true successor

        # If the node is ending before any of current successor, check if it's
        # score is higher (remove weaker successors)
        self.succ = set(
            s
            for s in self.succ
            if s.end < node.end or node.score <= s.score
        )

        # Add the node to our set of successors
        self._set_closest_end(node.end)
        self.succ.add(node)

        return False

    def _set_closest_end(self, value):
        """Remember the shortest successor's end value.

        If the '_succ_closest_end' is not set, save 'value' as is, find the
        minimal otherwise
        """
        # pylint: disable=method-hidden
        self._succ_closest_end = value
        self._set_closest_end = self.__set_closest_end

    def __set_closest_end(self, value):
        # Bypassing IF statement
        self._succ_closest_end = min(self._succ_closest_end, value)


def get_line(input_file):
    """Feature generator parsing input file."""
    with open(input_file, 'r') as f:
        # Start counting lines
        for line_no, line in enumerate(f, start=1):
            # Skip empty or commented lines
            if len(line) <= 1 or line[0] == '#':
                continue

            line = line.split()
            # Create an unique ID defining the strand type
            fid = "{0} {1} {2}".format(line[0], line[2], line[6])

            # Get me the line!
            yield fid, Feature(line_no, int(line[3]),
                               int(line[4]), int(line[5]))


def start_points(nodes):
    """Make a list of all nodes without a predecessor.

    Loop the 'nodes' and find such nodes which can't have a predecessor. Remove
    these from 'nodes' list.
    """
    prev_nodes = set()
    # Use the first node's end as a reference
    closest_end = nodes[0].end

    # Find all starting points
    for n in nodes:
        if n.start > closest_end:
            # Node can be a successor of closest_end's node
            break

        # Remember the node and adjust the limit
        prev_nodes.add(n)
        closest_end = min(n.end, closest_end)

        # we shouldn't use this node any more
        nodes.remove(n)

    return prev_nodes


def find_routes(input_file, is_sorted=False):
    """Create graph from Features, build Sequences and find the best one.

    View the Overlapping as a graph. Take each Feature as a node and connect it
    forward - with its successors. Then iterate through all starting nodes
    (they are not successors of any other node). For each such node, take it's
    successors and add this sequence to scope. Repeat with all these Sequences
    and find the best!
    """
    # pylint: disable=expression-not-assigned
    graph = defaultdict(set)
    best_routes = dict()

    # Init graph nodes (Features)
    [graph[t].add(f) for t, f in get_line(input_file)]

    # Loop each strand type separately
    for t, nodes in graph.items():
        # Sort nodes
        nodes = sorted(nodes, key=attrgetter('start', 'end'))

        # Remember the top score route (set the best node to begin with)
        x = max(nodes, key=attrgetter('score'))
        t_best = Sequence(x.score, x.start, x.end, (x,))

        # Start the search from nodes with no predecessors
        for p in start_points(nodes):
            # Generate all successors of starting points
            p.gen_successors(nodes, is_in_list=False)

            # Generate initial routes
            routes = set(
                Sequence(p.score+s.score, p.start, s.end, (p, s))
                for s in p.succ
            )

            # Find the best route
            while routes:
                # Get the longest route (so we can eliminate them first)
                r = routes.pop()

                # Skip the route if current best is already shorter but better
                if t_best < r and r << t_best:
                    continue

                # Remove further routes which would be worse than this one
                routes.difference_update(
                    k
                    for k in routes.copy()
                    if r < k and k << r
                )

                # Generate successors for the last node in route
                r.nodes[-1].gen_successors(nodes)

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

    return best_routes


def print_gff(input_file, data_dict):
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
        for node in series.nodes:
            # Find the line in input_file and print it
            line = getline(input_file, node.line)
            print(line, end="")


if __name__ == '__main__':
    try:
        s_mode = sys.argv[1] == '-s'
        best_sequences = find_routes(sys.argv[-1], is_sorted=s_mode)
        print_gff(sys.argv[-1], best_sequences)
    except IndexError:
        print('Missing input file')
