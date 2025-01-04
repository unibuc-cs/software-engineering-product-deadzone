#include "Random.h"

double Random::random01()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    return dis(gen);
}

int Random::randomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

std::mt19937 Random::randomGen()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	return gen;
}

