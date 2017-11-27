#pragma once

#include <cstdint>
#include <random>
#include <tuple>

class Model
{
public:
    Model(float hourly_arrivals = 20.0f, float processing = 1.0f / 3.0f);
    ~Model();

    using Result = std::tuple<uint32_t, uint32_t, float>;

    Result Run(float duration);

protected:

    using Update = std::tuple<float, uint32_t, uint32_t, uint32_t, float>;

    Update Step(uint32_t waiting, float max_delta_t);

    float arrival_rate;
    float processing_rate;
    std::mt19937_64 generator;
};
