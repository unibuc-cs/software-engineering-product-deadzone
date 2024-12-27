#include "Random.h"

double Random::random01()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    return dis(gen);
}

int Random::randomInt(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

std::mt19937 Random::randomGen() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}

