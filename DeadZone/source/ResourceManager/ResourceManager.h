#pragma once

#include <glad/glad.h>

#include <map>
#include <string>

#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Flipbook.h"

#include <fmod.hpp>

class ResourceManager
{
public:
	// resource storage
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;
	static std::map<std::string, Font> fonts;
	static std::map<std::string, Flipbook> flipbooks;
	static std::map<std::string, FMOD::Sound*> sounds;

	// load and generate a shader program from files: vertex, fragment, geometry
	static void loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name);

	// retrieve a stored shader
	static Shader& getShader(const std::string& name);

	// load and generate a texture from file
	static void loadTexture(const char* file, const bool& alpha, const std::string& name);

	// retrieve a stored texture
	static Texture2D& getTexture(const std::string& name);

	// load and generate font textures
	static void loadFont(const char* fontFilePath, const unsigned int fontSize, const std::string& name);

	// retrieve a stored font
	static Font& getFont(const std::string& name);

	// load and genereate flipbook
	static void loadFlipbook(const char* directoryPath, const double& framesPerSecond, bool loop, const std::string& name);

	// retrieve a stored flipbook
	static Flipbook& getFlipbook(const std::string& name);

	// load a new sound
	static void loadSound(const char* file, FMOD_MODE mode, const std::string& name);

	// retrieve a stored sound
	static FMOD::Sound* getSound(const std::string& name);

	// de-allocate all resources
	static void clear();

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	ResourceManager(const ResourceManager& other) = delete;
	ResourceManager& operator= (const ResourceManager& other) = delete;
	ResourceManager(const ResourceManager&& other) = delete;
	ResourceManager& operator= (const ResourceManager&& other) = delete;
};

