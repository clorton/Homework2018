#pragma once

#include <cstdint>
#include <queue>
#include <set>
#include <string>
#include <tuple>

class Simulation
{
public:
    Simulation(uint32_t number_of_chairs);
    ~Simulation();

    uint32_t SeatNext();
    uint32_t FreeNext();

    std::string ToString();

protected:

    using Block = std::tuple<uint32_t, uint32_t, uint32_t>;   // size, start, end

    uint32_t GetNextChair();
    bool GetAdjacentLeft(uint32_t chair, Block& left);
    bool GetAdjacentRight(uint32_t chair, Block& right);

    // This compare functor for use with std::set will put larger blocks at the beginning of the set.
    struct compare
    {
        bool operator()(const Block& a, const Block& b)
        {
            auto[size_a, start_a, _] = a;
            auto[size_b, start_b, _] = b;
            return (size_a > size_b) || ((size_a == size_b) && (start_a < start_b));
        }
    };

    uint32_t capacity;
    char* state;
    std::queue<uint32_t> queue;
    std::set<Block, Simulation::compare> pool;
};
