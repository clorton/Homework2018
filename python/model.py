#! /usr/bin/python

from __future__ import print_function
from enum import Enum
import argparse
import math
import numpy
import sys

"""
The waiting area for a large hospital consists of one long row of chairs. Due to concerns about communicable diseases,
incoming patients prefer to choose a seat as far as possible from any currently waiting patients. Information from the
hospital lets us know the average number of patients arriving per hour and the average rate at which patients are
processed (removed from the waiting room) in patients per minute. The total number of available chairs, the average
number of patients per hour, the average rate of processing in patients per minute, the duration of the simulation, and
the length of simulation time step should be configurable for your simulation. Your simulation should output the current
state of the waiting room (available and occupied chairs) as well as information about arriving and departing patients
for analysis.
"""

ARRIVAL_SPAN = 60.0     # arrivals are averaged over 60 minutes


class Event(Enum):
    ARRIVAL = 1
    DEPARTURE = 2


class Simulation(object):

    def __init__(self, arrival, departure):
        self._average_arrivals = arrival
        self._arrival_rate = arrival / ARRIVAL_SPAN
        self._departure_rate = departure
        return

    @property
    def average_arrivals(self):
        return self._average_arrivals

    @property
    def departure_rate(self):
        return self._departure_rate

    def run(self, duration):

        total_patients = 0
        total_processed = 0
        total_processing_time = 0.0

        waiting = 0
        current_time = 0.0
        while current_time < duration:
            delta_t, waiting, arrivals, processed, processing_time = self.step(waiting, duration - current_time)
            current_time += delta_t
            total_patients += arrivals
            total_processed += processed
            total_processing_time += processing_time
            print('[{0:07.2f}] {1} processed & {2:3} arrivals in {3:6.2f} minutes - {4:3} people currently waiting'.format(current_time, processed, arrivals, delta_t, waiting))

        return total_patients, total_processed, total_processing_time

    def step(self, waiting, max_delta_t):

        if waiting > 0:
            # Someone is in the queue, take that person out for processing
            waiting -= 1
            processed = 1
            # Exponential draw, based on departure rate, for how long processing takes
            processing_time = -math.log(numpy.random.random()) / self._departure_rate
            delta_t = processing_time
            # Poisson draw, based on average arrivals, for the # of people who arrive while this person is processed
            arrivals = numpy.random.poisson(min(delta_t, max_delta_t) * self._arrival_rate)
        else:
            # No one is in the queue
            processed = 0
            processing_time = 0.0
            # Skip ahead in time until the arrival of the next person, exponential draw based on arrival rate
            delta_t = -math.log(numpy.random.random()) / self._arrival_rate
            if delta_t < max_delta_t:
                arrivals = 1
            else:
                arrivals = 0
                delta_t = max_delta_t

        waiting += arrivals

        return delta_t, waiting, arrivals, processed, processing_time


def main(average_arrivals, departure_rate, simulation_duration):

    print('Average patients arriving per hour:    {0:.2f}'.format(average_arrivals))
    print('Rate of patients processed per minute: {0:.2f}'.format(departure_rate))
    print('Simulation duration:                   {0}'.format(simulation_duration))
    print()

    simulation = Simulation(average_arrivals, departure_rate)
    total_patients, total_processed, total_processing_time = simulation.run(simulation_duration)

    print()
    print('Arrivals simulated:        {0}'.format(total_patients))
    print('Average patients per hour: {0:.2f}'.format(total_patients * 60.0 / simulation_duration))
    print('Patients processed:        {0}'.format(total_processed))
    print('Average processing time:   {0:.2f}'.format(total_processing_time / total_processed))

    return


DEFAULT_ARRIVALS = 20.0
DEFAULT_PROCESSING = 1.0/3.0
DEFAULT_DURATION = 48 * 60.0


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--arriving', type=float, default=DEFAULT_ARRIVALS, help='Average number of patients arriving per hour [{0:.2f}].'.format(DEFAULT_ARRIVALS))
    parser.add_argument('-p', '--processing', type=float, default=DEFAULT_PROCESSING, help='Rate at which patients are processed (#/minute) [{0:.2f}].'.format(DEFAULT_PROCESSING))
    parser.add_argument('-d', '--duration', type=float, default=DEFAULT_DURATION, help='Duration of the simulation in minutes [{0}].'.format(DEFAULT_DURATION))

    args = parser.parse_args()

    main(args.arriving, args.processing, args.duration)

    sys.exit(0)
