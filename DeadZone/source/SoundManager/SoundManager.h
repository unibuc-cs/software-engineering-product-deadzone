#pragma once

#include <string>
#include <map>

#include <fmod.hpp>

class SoundManager
{
public:
	static SoundManager& get();

	void play(const std::string& name, bool paused);
	void pause(const std::string& name);
	void resume(const std::string& name);
	bool isPlaying(const std::string& name);

	inline FMOD::System* getSystem() { return fmodSystem; }

private:
	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator= (const SoundManager& other) = delete;
	SoundManager(const SoundManager&& other) = delete;
	SoundManager& operator= (const SoundManager&& other) = delete;

	static FMOD::System* fmodSystem;

	std::map<std::string, FMOD::Channel*> channels;
};

