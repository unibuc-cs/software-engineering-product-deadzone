#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "../Random/Random.h"
#include <chrono>

class GeneralUtilities
{
public:
	GeneralUtilities();
	~GeneralUtilities();

public:
	static GeneralUtilities& get();
	void generatePerlinMap(const int& width, const int& height, const double& scale, const int& hash_size, std::vector<std::vector<std::string>>& map);
	long long getTimeSinceEpochInMs();
};

