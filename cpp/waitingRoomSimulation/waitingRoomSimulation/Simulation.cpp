#include "stdafx.h"
#include "Simulation.h"

#define OCCUPIED    ('X')
#define AVAILABLE   ('-')

Simulation::Simulation(uint32_t number_of_chairs)
    : capacity(number_of_chairs)
    , state(new char[capacity])
    , queue()
    , pool()
{
    memset(state, AVAILABLE, capacity);         // Mark all chairs as available.
    pool.insert({ capacity, 0, capacity - 1 }); // Initialize the pool with a single block of all the chairs.
}

Simulation::~Simulation()
{
    delete[] state;
}

uint32_t Simulation::SeatNext()
{
    uint32_t chair = GetNextChair();
    state[chair] = OCCUPIED;
    queue.push(chair);

    return chair;
}

#define BLOCK_SIZE(_blk)    (std::get<0>(_blk))
#define BLOCK_START(_blk)   (std::get<1>(_blk))
#define BLOCK_END(_blk)     (std::get<2>(_blk))

#define FIRST_CHAIR (0)
#define LAST_CHAIR  (capacity - 1)

uint32_t Simulation::FreeNext()
{
    uint32_t chair;

    if (queue.size() > 0)
    {
        chair = queue.front();
        queue.pop();
        state[chair] = AVAILABLE;
        Block block = { 1, chair, chair };
        Block adjacent;

        // If there is an adjacent block to the left, merge with this block.
        if (GetAdjacentLeft(chair, adjacent))
        {
            uint32_t size = BLOCK_SIZE(adjacent) + BLOCK_SIZE(block);
            block = { size, BLOCK_START(adjacent), BLOCK_END(block) };
        }
        // If there is an adjacent block of free chairs to the right, merge with this block.
        if (GetAdjacentRight(chair, adjacent))
        {
            uint32_t size = BLOCK_SIZE(block) + BLOCK_SIZE(adjacent);
            block = { size, BLOCK_START(block), BLOCK_END(adjacent) };
        }

        // If the block is at either end of the line, increase its effective size.
        if ((BLOCK_START(block) == FIRST_CHAIR) || (BLOCK_END(block) == LAST_CHAIR)) {
            block = { 2 * BLOCK_SIZE(block) - 1, BLOCK_START(block), BLOCK_END(block) };
        }

        pool.insert(block);
    }
    else {
        throw "Asked to free a chair but none are occupied.";
    }

    return chair;
}

uint32_t Simulation::GetNextChair()
{
    uint32_t chair;

    // Get biggest contiguous block of free chairs from the pool
    if (pool.size() > 0)
    {
        auto iterator = pool.begin();
        auto block = *iterator;
        pool.erase(iterator);

        // Blocks at the ends of the line get special treatment.
        if (BLOCK_START(block) == FIRST_CHAIR) {
            chair = FIRST_CHAIR;
        }
        else if (BLOCK_END(block) == LAST_CHAIR) {
            chair = LAST_CHAIR;
        }
        else {
            chair = (BLOCK_START(block) + BLOCK_END(block)) / 2;
        }

        if (chair > BLOCK_START(block))
        {
            uint32_t size = chair - BLOCK_START(block);
            // Special case blocks at the start of the line.
            if (BLOCK_START(block) == FIRST_CHAIR) {
                size = (2 * size) - 1;
            }
            pool.insert({ size, BLOCK_START(block), chair - 1 });
        }

        if (chair < BLOCK_END(block))
        {
            uint32_t size = BLOCK_END(block) - chair;
            // Special case blocks at the end of the line.
            if (BLOCK_END(block) == LAST_CHAIR) {
                size = (2 * size) - 1;
            }
            pool.insert({ size, chair + 1, BLOCK_END(block) });
        }
    }
    else {
        throw "Asked to seat a patient, but no chairs are available.";
    }

    return chair;
}

bool Simulation::GetAdjacentLeft(uint32_t chair, Simulation::Block& left)
{
    uint32_t test = chair - 1;
    for (auto it = pool.begin(); it != pool.end(); ++it)
    {
        if (BLOCK_END(*it) == test)
        {
            left = *it;
            pool.erase(it);
            return true;
        }
    }

    return false;
}

bool Simulation::GetAdjacentRight(uint32_t chair, Simulation::Block& right)
{
    uint32_t test = chair + 1;
    for (auto it = pool.begin(); it != pool.end(); ++it)
    {
        if (BLOCK_START(*it) == test)
        {
            right = *it;
            pool.erase(it);
            return true;
        }
    }

    return false;
}

std::string Simulation::ToString()
{
    return std::string(state, size_t(capacity));
}
