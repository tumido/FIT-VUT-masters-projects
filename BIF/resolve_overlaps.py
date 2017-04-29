#!/usr/bin/env python3
import sys
from collections import namedtuple
from pprint import pprint
from copy import copy
from datetime import date

# Create an object for each entry in the file
Feature = namedtuple(
    'Feature',
    ['fid', 'seq_id', 'source', 'type', 'start',
     'end', 'score', 'strand', 'phase']
)


def get_line(input_file):
    """Feature generator parsing input file."""
    with open(input_file, 'r') as f:
        for line in f:
            # Skip empty or commented lines
            if len(line) <= 1 or line[0] == '#':
                continue

            line = line.split()
            # Create an unique ID
            fid = "{0} {1} {2}".format(line[0], line[2], line[6])

            # Convert 'start', 'end' and 'score' to integer
            line[3:6] = list(map(int, line[3:6]))

            # Get me the line!
            yield Feature(fid, *line)


def is_overlap(sequence, feature):
    # print(sequence)
    # Loop through sequence and find out if there's any overlapping feature
    for f in sequence:
        # If the later starting one is before the end of the other, it's wrong
        if max(f.start, feature.start) <= min(f.end, feature.end):
            return True
    return False


def optimize(sequences, f):
    better_exists = False
    worse_lst = list()
    for s in filter(lambda x: len(x) == 1, sequences):
        s = s[0]
        # Find a shorter Feature in the same region which is better than 'f'
        if s.start >= f.start and s.end <= f.end and f.score < s.score:
            better_exists = True
        # Find a longer Feature in the same region which is worse than 'f'
        if s.start <= f.start and s.end >= f.end and f.score > s.score:
            worse_lst.append(s)

    return better_exists, worse_lst


def build_sequences(input_file):
    sequences = dict()
    i = 0
    # Process each Feature in input file
    for f in get_line(input_file):
        i += 1
        # If the Feature is first of its type, add a new key
        if f.fid not in sequences.keys():
            sequences[f.fid] = list()

        stat = " ".join("{0}: {1}".format(k, len(v)) for k, v in sequences.items())
        print("Item: {0:10} {1}".format(i, stat), end='\r')
        # Skip Features which are longer and have a worse score than already
        # listed ones
        is_worse, substitute = optimize(sequences[f.fid], f)
        if is_worse:
            continue
        for to_del in substitute:
            sequences[f.fid] = list(filter(lambda x: to_del not in x, sequences[f.fid]))

        # Find already existent sequences where's no overlap
        no_overlaps = filter(lambda x: not is_overlap(x, f), sequences[f.fid])
        elongated = map(lambda x: copy(x) + [f], no_overlaps)

        # Join all the sequences for this type and the ones without overlap
        # (cloned + new Feature added)
        sequences[f.fid] = list(elongated) + sequences[f.fid]

        sequences[f.fid].append([f])
        if i > 500:
            break
    print()
    return sequences


def best_score(seqs):
    return max(seqs, key=lambda s: sum(x.score for x in s))


def print_gff(meta, data):
    print('##gff-version 3')
    print('##date %s' % date.today())
    for group, score in meta:
        print("# Best score of '{0}': {1}".format(group, score))
    for line in data:
        print('\t'.join(map(str, line[1:])))


if __name__ == '__main__':
    try:
        all_sequences = build_sequences(sys.argv[1])

        output = list()
        scores = list()
        for fid, sequences in all_sequences.items():
            best = best_score(sequences)
            scores.append((fid, sum(x.score for x in best)))
            output += best

        print_gff(scores, output)

    except IndexError:
        print('Missing input file')
