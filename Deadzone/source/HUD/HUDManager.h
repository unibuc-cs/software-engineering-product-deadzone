#pragma once

class HUDManager
{
public:
	static HUDManager& get();

	void draw();

private:
	HUDManager() = default;
	~HUDManager() = default;

	HUDManager(const HUDManager& other) = delete;
	HUDManager& operator= (const HUDManager& other) = delete;
	HUDManager(const HUDManager&& other) = delete;
	HUDManager& operator= (const HUDManager&& other) = delete;
};

