#!/usr/bin/env python3
import sys
from collections import namedtuple, defaultdict
from operator import attrgetter as get
from copy import copy
from datetime import date

Sequence = namedtuple('Sequence', ['score', 'start', 'end', 'nodes'])

class Feature:
    def __init__(self, fid, line, start, end, score):
        self.fid = fid
        self.line = line
        self.start = start
        self.end = end
        self.score = score
        self.succ = set()
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
            print("Processing line: %d" % i, end='\r')
            yield fid, Feature(fid, line, int(l[3]), int(l[4]), int(l[5]))


def exists_better(route, all_routes):
    for r in all_routes:
        if route.start <= r.start and r.end <= route.start and route.score < r.score:
            return True
    return False

def collect_worse(route, all_routes):
    to_remove = set()
    for r in all_routes:
        if r.start <= route.start and route.end <= r.end and route.score > r.score:
            to_remove.add(r)
    all_routes.difference_update(to_remove)


def connect_nodes(nodes):
    best_node = None
    for node in nodes:
        # Find the valid successors
        [node.make_succ(n) for n in nodes]
        # Keep track of the best score
        if not best_node or node.score > best_node.score:
            best_node = Sequence(node.score, node.start, node.end, (node,))
    return best_node


def find_routes(input_file):
    graph = defaultdict(set)
    best_routes = defaultdict(tuple)

    # Init Graph nodes (Features)
    [graph[t].add(f) for t, f in get_line(input_file)]

    # Loop each strand type separately
    for t, nodes in graph.items():
        # Place the node into the graph
        # best_routes[t] = connect_nodes(nodes)
        for node in nodes:
            # Find the valid successors
            [node.make_succ(n) for n in nodes]
            # Keep track of the best score
            if not best_routes[t] or node.score > best_routes[t].score:
                best_routes[t] = Sequence(node.score, node.start, node.end, (node,))
        # Find proper starting places
        start_points = set()
        for node in nodes:
            for n in nodes:
                if node in n.succ:
                    break
            else:
                start_points.add(node)

        # Start the search from nodes with no predecessors
        for p in start_points:
            # Find the best route
            possible_routes = set(
                Sequence(p.score+s.score, p.start, s.end, (p, s))
                for s in p.succ
            )
            while possible_routes:
                print(
                    "All possible routes: {0:10}, Top score: {1:10}".format(
                        len(possible_routes), best_routes[t].score),
                    end='\r'
                )
                r = possible_routes.pop()
                collect_worse(r, possible_routes)
                # if r.start <= best_routes[t].start and best_routes[t].end <= r.end and r.score < best_routes[t].score:
                #     continue
                if exists_better(r, possible_routes):
                    continue

                if not r.nodes[-1].succ:
                    if r.score > best_routes[t].score:
                        best_routes[t] = r
                    continue

                possible_routes.update(set(
                    Sequence(r.score+n.score, r.start, n.end, (*r.nodes, n))
                    for n in r.nodes[-1].succ
                ))
        print()

    return best_routes


def print_gff(meta, data):
    print('##gff-version 3')
    print('##date %s' % date.today())
    for group, score in meta:
        print("# Best score of '{0}': {1}".format(group, score))
    for r in data:
        for node in r.nodes:
            print(node.line, end="")


if __name__ == '__main__':
    try:
        best_routes = find_routes(sys.argv[1])

        output = list()
        scores = list()
        for fid, route in best_routes.items():
            scores.append((fid, route.score))
            output.append(route)

        print_gff(scores, output)

    except IndexError:
        print('Missing input file')
