// waitingRoomSimulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <streambuf>
#include <string>
#include "Simulation.h"

struct Event {
    float timeStamp;
    uint32_t nProcessed;
    uint32_t nArrivals;
    float timeStep;
    uint32_t nWaiting;
};

std::list<Event> LoadData(const std::string& filename);

int main(uint32_t argc, char**argv)
{
    std::string filename("output.txt");
    uint32_t capacity = 40;

    for (size_t iarg = 1; iarg < argc; ++iarg)
    {
        if (argv[iarg][0] == '-')
        {
            switch (argv[iarg][1])
            {
            case 'f':
                if (++iarg < argc) {
                    filename = argv[iarg];
                    std::cout << "Using '" << filename.c_str() << "' for data." << std::endl;
                }
                else {
                    std::cout << "Missing value for option '-f'." << std::endl;
                }
                break;

            case 'n':
                if (++iarg < argc) {
                    capacity = atoi(argv[iarg]);
                    std::cout << "Simulating " << capacity << " chairs." << std::endl;
                }
                else {
                    std::cout << "Missing value for option '-n'." << std::endl;
                }
                break;

            default:
                std::cout << "Unrecognized option '" << argv[iarg] << "'." << std::endl;
            }
        }
    }

    auto events = LoadData(filename);
    Simulation sim(capacity);

    for (auto event : events)
    {
        if (event.nProcessed > 0)
        {
            uint32_t chair = sim.FreeNext();
            std::cout << "Processed patient in chair #" << chair << std::endl;
            std::cout << sim.ToString().c_str() << std::endl;
        }
        for (size_t i = 0; i < event.nArrivals; ++i)
        {
            uint32_t chair = sim.SeatNext();
            std::cout << "Seated patient in chair #" << chair << std::endl;
            std::cout << sim.ToString().c_str() << std::endl;
        }
    }

    return 0;
}

#define ITIMESTAMP  (1)
#define IPROCESSED  (2)
#define IARRIVALS   (3)
#define ITIMESTEP   (4)
#define IWAITING    (5)

std::list<Event> LoadData(const std::string& filename)
{
    std::list<Event> events;

    // [1441.24] 1 processed &   1 arrivals in   1.47 minutes -   9 people currently waiting
    std::regex regex("^\\[ *([0-9\\.]+)\\] ([0-9]+) processed \\& +([0-9]+) arrivals in +([0-9\\.])+ minutes - +([0-9]+) people currently waiting$");
    std::smatch matches;
    std::ifstream file(filename);
    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        if (std::regex_match(line, matches, regex))
        {
            float timeStamp = float(atof(matches[ITIMESTAMP].str().c_str()));
            uint32_t nProcessed = atoi(matches[IPROCESSED].str().c_str());
            uint32_t nArrivals = atoi(matches[IARRIVALS].str().c_str());
            float timeStep = float(atof(matches[ITIMESTEP].str().c_str()));
            uint32_t nWaiting = atoi(matches[IWAITING].str().c_str());
            events.push_back({ timeStamp, nProcessed, nArrivals, timeStep, nWaiting });
        }
    }

    return events;
}