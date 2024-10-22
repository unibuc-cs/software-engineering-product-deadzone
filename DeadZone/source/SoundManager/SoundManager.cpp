#include "SoundManager.h"

#include <iostream> // TODO: debug

#include "../ResourceManager/ResourceManager.h"

// instantiate static variables
FMOD::System* SoundManager::fmodSystem = nullptr;

SoundManager::SoundManager()
{
	// Initialize Sound System
	if (FMOD::System_Create(&fmodSystem) != FMOD_OK)
	{
		std::cout << "ERROR : SOUND SYSTEM NOT CREATED!" << std::endl;
	}
	else
	{
		std::cout << "SOUND SYSTEM CREATED!" << std::endl;
		fmodSystem->init(32, FMOD_INIT_NORMAL, NULL);
	}
}

SoundManager::~SoundManager()
{
	fmodSystem->close();
	fmodSystem->release();
}

SoundManager& SoundManager::get()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::play(const std::string& name, bool paused)
{
	fmodSystem->playSound(ResourceManager::getSound(name), nullptr, paused, &channels[name]);
}

void SoundManager::pause(const std::string& name)
{
	channels[name]->setPaused(true);
}

void SoundManager::resume(const std::string& name)
{
	channels[name]->setPaused(false);
}

bool SoundManager::isPlaying(const std::string& name)
{
	bool playing;
	channels[name]->isPlaying(&playing);
	return playing;
}

