#include "SoundManager.h"

#include <iostream> // TODO: debug

#include "../ResourceManager/ResourceManager.h"
#include "../Client/Client.h"

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

void SoundManager::play(const std::string& name, bool paused, bool multiplayer)
{
	fmodSystem->playSound(ResourceManager::getSound(name), nullptr, paused, &channels[name]);

	// TODO: muta nu vreau sa trimit orice fel de sunet - trimite la catre ceilalti clienti
	if (multiplayer)
	{
		Client::get().sendSound(name, paused);
	}
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

