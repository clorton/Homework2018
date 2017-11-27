#include "stdafx.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "Model.h"

#define MINUTES_PER_HOUR    60

Model::Model(float hourly_arrivals, float processing)
    : arrival_rate(hourly_arrivals / MINUTES_PER_HOUR)
    , processing_rate(processing)
    , generator(std::chrono::system_clock::now().time_since_epoch().count())
{
}

Model::Result Model::Run(float duration)
{
    uint32_t total_patients = 0;
    uint32_t total_processed = 0;
    float total_processing_time = 0.0;

    uint32_t waiting = 0;
    float current_time = 0.0;
    while (current_time < duration)
    {
        auto [ delta_t, now_waiting, arrivals, processed, processing_time ] = Step(waiting, duration - current_time);
        current_time += delta_t;
        waiting = now_waiting;
        total_patients += arrivals;
        total_processed += processed;
        total_processing_time += processing_time;
        // print('[{0:07.2f}] {1} processed & {2:3} arrivals in {3:6.2f} minutes - {4:3} people currently waiting'.format(current_time, processed, arrivals, delta_t, waiting))
        std::cout << std::fixed << "[" << std::setw(7) << std::setprecision(2) << current_time << "] "
            << processed << " processed & "
            << std::setw(3) << arrivals << " arrivals in "
            << std::setw(6) << std::setprecision(2) << delta_t << " minutes - "
            << std::setw(3) << waiting << " people currently waiting"
            << std::endl;
    }

    return { total_patients, total_processed, total_processing_time };
}

Model::Update Model::Step(uint32_t waiting, float max_delta_t)
{
    uint32_t processed;
    float processing_time;
    float delta_t;
    uint32_t arrivals;

    if (waiting > 0)
    {
        // Someone is in the queue, take that person out for processing
        waiting -= 1;
        processed = 1;
        // Exponential draw, based on departure rate, for how long processing takes
        std::exponential_distribution<float> exponential(processing_rate);
        processing_time = exponential(generator);
        delta_t = processing_time;
        // Poisson draw, based on average arrivals, for the # of people who arrive while this person is processed
        std::poisson_distribution<int> poisson(std::min(delta_t, max_delta_t) * arrival_rate);
        arrivals = poisson(generator);
    }
    else
    {
        // No one is in the queue
        processed = 0;
        processing_time = 0.0;
        // Skip ahead in time until the arrival of the next person, exponential draw based on arrival rate
        std::exponential_distribution<float> exponential(arrival_rate);
        delta_t = exponential(generator);
        if (delta_t < max_delta_t)
        {
            arrivals = 1;
        }
        else
        {
            arrivals = 0;
            delta_t = max_delta_t;
        }
    }

    waiting += arrivals;

    return { delta_t, waiting, arrivals, processed, processing_time };
}

Model::~Model()
{
}
