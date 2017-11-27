#! /usr/bin/python

from collections import deque, namedtuple
from heapq import heappush, heappop, heapify
import argparse
import sys
import analysis

Block = namedtuple('Block', ['priority', 'size', 'start', 'end'])
AVAILABLE = '-'
OCCUPIED = 'X'


class State(object):
    def __init__(self, number_of_chairs):
        self._capacity = number_of_chairs
        self._state = [AVAILABLE] * number_of_chairs
        self._queue = deque()
        self._pool = [Block(0, number_of_chairs, 0, number_of_chairs-1)]
        pass

    def seat_next(self):
        chair = self._get_next_chair()
        self._state[chair] = OCCUPIED
        self._queue.append(chair)

        return chair

    def _get_next_chair(self):
        # get biggest contiguous block from pool
        if len(self._pool) > 0:
            block = heappop(self._pool)
            # split block at midpoint (return value = midpoint)
            midpoint = int((block.start + block.end) / 2)
            # put left & right blocks into pool
            if midpoint > block.start:
                size = midpoint - block.start
                heappush(self._pool, Block(self._capacity - size, size, block.start, midpoint - 1))
            if midpoint < block.end:
                size = block.end - midpoint
                heappush(self._pool, Block(self._capacity - size, size, midpoint + 1, block.end))
        else:
            raise RuntimeError('Asked to seat a patient, but no chairs are available.')

        return midpoint

    def free_next(self):
        if len(self._queue) > 0:
            chair = self._queue.popleft()
            self._state[chair] = AVAILABLE
            new_block = Block(self._capacity - 1, 1, chair, chair)
            left = self._get_adjacent_left(chair)
            if left:
                new_size = left.size + new_block.size
                new_block = Block(self._capacity - new_size, new_size, left.start, new_block.end)
            right = self._get_adjacent_right(chair)
            if right:
                new_size = new_block.size + right.size
                new_block = Block(self._capacity - new_size, new_size, new_block.start, right.end)
            heappush(self._pool, new_block)
        else:
            raise RuntimeError('Asked to free a chair but none are occupied.')

        return chair

    def _get_adjacent_left(self, chair):
        adjacent = None
        test = chair - 1
        for i in range(len(self._pool)):
            if self._pool[i].end == test:
                adjacent = self._pool.pop(i)
                heapify(self._pool)
                break

        return adjacent

    def _get_adjacent_right(self, chair):
        adjacent = None
        test = chair + 1
        for i in range(len(self._pool)):
            if self._pool[i].start == test:
                adjacent = self._pool.pop(i)
                heapify(self._pool)
                break

        return adjacent

    def display(self):
        print(''.join(self._state))
        return


def main(filename, number_of_chairs):

    model_data = analysis.process_file(filename)
    state = State(number_of_chairs)

    for event in model_data:
        if event.processed > 0:
            chair = state.free_next()
            print('Processed patient in chair #{0}'.format(chair))
            state.display()
        for i in range(int(event.arrivals)):
            chair = state.seat_next()
            print('Seated patient in chair #{0}'.format(chair))
            state.display()

    return


DEFAULT_FILENAME = 'output.txt'
FILENAME_HELP = 'Model output filename [{0}]'.format(DEFAULT_FILENAME)
DEFAULT_NUMBER = 40
NUMBER_HELP = 'Number of chairs [{0}]'.format(DEFAULT_NUMBER)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--filename', default=DEFAULT_FILENAME, help=FILENAME_HELP)
    parser.add_argument('-n', '--number', default=40, type=int, help=NUMBER_HELP)
    args = parser.parse_args()

    main(args.filename, args.number)

    sys.exit(0)
