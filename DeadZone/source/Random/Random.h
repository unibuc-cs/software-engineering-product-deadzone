#pragma once
#include <random>

namespace Random
{
    const double EPSILON = 0.05;

    double random01();
	int randomInt(int min, int max);
    std::mt19937 randomGen();
};