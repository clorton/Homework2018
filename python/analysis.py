#! /usr/bin/python

from matplotlib import pyplot
import argparse
import re
import sys


class Foo(object):
    def __init__(self, m):
        for group in m.re.groupindex:
            self.__setattr__(group, float(m.group(group)))


def main(filename):

    model_data = process_file(filename)

    arrival_data = [entry.arrivals / entry.time_step for entry in model_data if (entry.processed == 1.0) and (entry.arrivals > 0.0)]
    wait_times = [entry.time_step for entry in model_data if entry.processed == 1.0]

    pyplot.hist(arrival_data, bins=256)
    pyplot.show()

    pyplot.hist(wait_times, bins=32)
    pyplot.show()

    return


def process_file(filename):

    with open(filename, 'r') as handle:
        lines = handle.readlines()

    regex = '^' \
            '\[(?P<time_stamp>[0-9\.]+)\] ' \
            '(?P<processed>[0-9]+) processed \& +' \
            '(?P<arrivals>[0-9]+) arrivals in *' \
            '(?P<time_step>[0-9\.]+) minutes \- *' \
            '(?P<queued>[0-9]+) people currently waiting' \
            '$'

    data = []
    pattern = re.compile(regex)
    for line in lines:
        m = pattern.match(line)
        if m:
            data.append(Foo(m))

    return data


DEFAULT_FILENAME = 'output.txt'
FILENAME_HELP = 'Simulation output filename [{0}]'.format(DEFAULT_FILENAME)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--filename', default=DEFAULT_FILENAME, help=FILENAME_HELP)
    args = parser.parse_args()
    main(args.filename)

    sys.exit(0)
