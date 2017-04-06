"""
Plain server for study cards
"""
from yaml import load
from sys import argv
from random import shuffle

if __name__ == "__main__":
    with open(argv[1], 'r') as source:
        data = list(load(source).items())

    shuffle(data)

    for index, (key, value) in enumerate(data):
        input("({0}/{1})\t{2}?".format(index+1, len(data), key))

        if isinstance(value, str):
            value = (value,)
        for i in value:
            print(" *  {}".format(i))
        print("\n\n")
