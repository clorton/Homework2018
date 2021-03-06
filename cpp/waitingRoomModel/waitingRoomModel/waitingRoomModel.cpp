// waitingRoomModel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Model.h"

int main(int argc, char** argv)
{
    float hourly_arrivals = 20.0f;          // 20 per hour
    float average_processing = 1.0f / 3.0f; // 1 every three minutes
    float duration = 60.0f*48.0f;           // 48 hours

    for (size_t iarg = 1; iarg < argc; ++iarg) {
        if (argv[iarg][0] == '-') {
            switch (argv[iarg][1]) {
            case 'a':
                ++iarg;
                if (iarg < argc) {
                    hourly_arrivals = float(atof(argv[iarg]));
                    std::cout << "Setting patients/hour to " << hourly_arrivals << std::endl;
                }
                else {
                    std::cout << "Missing value for '-a' option." << std::endl;
                }
                break;

            case 'd':
                ++iarg;
                if (iarg < argc) {
                    duration = float(atof(argv[iarg]));
                    std::cout << "Setting duration to " << duration << std::endl;
                }
                else {
                    std::cout << "Missing value for '-d' option." << std::endl;
                }
                break;

            case'p':
                ++iarg;
                if (iarg < argc) {
                    average_processing = float(atof(argv[iarg]));
                    std::cout << "Setting processing rate to " << average_processing << std::endl;
                }
                else {
                    std::cout << "Missing value for '-p' option." << std::endl;
                }
                break;

            default:
                std::cout << "Unknown option: " << argv[iarg] << std::endl;
            }
        }
    }

    std::cout << "Average patients arriving per hour: " << hourly_arrivals << std::endl;
    std::cout << "Patients processed per minute:      " << average_processing << std::endl;
    std::cout << "Simulation duration:                " << duration << std::endl;
    std::cout << std::endl;

    auto model = new Model(hourly_arrivals, average_processing);
    auto [ total_patients, total_processed, total_processing_time ] = model->Run(duration);

    std::cout << std::endl;
    std::cout << "Arrivals simulated:        " << total_patients << std::endl;
    std::cout << "Average patients per hour: " << (total_patients * 60.00 / duration) << std::endl;
    std::cout << "Patients processed:        " << total_processed << std::endl;
    std::cout << "Average processing time:   " << (total_processing_time / total_processed) << std::endl;

    return 0;
}

