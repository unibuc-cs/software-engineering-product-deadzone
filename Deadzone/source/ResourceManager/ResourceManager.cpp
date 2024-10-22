#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../SoundManager/SoundManager.h"

// instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Font> ResourceManager::fonts;
std::map<std::string, Flipbook> ResourceManager::flipbooks;
std::map<std::string, FMOD::Sound*> ResourceManager::sounds;

void ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name)
{
	shaders[name] = Shader();

	// retrieve the vertex/fragment/geometry source code from file path
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	// open files
	std::ifstream vertexShaderFile(vShaderFile);
	if (vertexShaderFile.fail())
	{
		throw std::runtime_error("Cannot open file: " + std::string(vShaderFile));
	}

	std::ifstream fragmentShaderFile(fShaderFile);
	if (fragmentShaderFile.fail())
	{
		throw std::runtime_error("Cannot open file: " + std::string(fShaderFile));
	}

	// read file's buffer contents into streams
	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vertexShaderFile.rdbuf();
	fShaderStream << fragmentShaderFile.rdbuf();

	// close file handlers
	vertexShaderFile.close();
	fragmentShaderFile.close();

	// convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	// optional parameter
	if (gShaderFile)
	{
		std::ifstream geometryShaderFile(gShaderFile);
		if (geometryShaderFile.fail())
		{
			throw std::runtime_error("Cannot open file: " + std::string(gShaderFile));
		}

		std::stringstream gShaderStream;
		gShaderStream << geometryShaderFile.rdbuf();
		geometryShaderFile.close();
		geometryCode = gShaderStream.str();
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	// create shader object from source code
	shaders[name].compile(vShaderCode, fShaderCode, gShaderFile ? gShaderCode : nullptr);
}

Shader& ResourceManager::getShader(const std::string& name)
{
	if (shaders.find(name) == shaders.end())
	{
		std::cout << "Error ResourceManager: could not find the shader!\n";
	}

	return shaders[name];
}

void ResourceManager::loadTexture(const char* file, const bool& alpha, const std::string& name)
{
	// create texture object
	textures[name] = Texture2D();

	if (alpha)
	{
		textures[name].internalFormat = GL_RGBA;
		textures[name].imageFormat = GL_RGBA;
	}

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (data == nullptr)
	{
		throw std::runtime_error("Cannot open file: " + std::string(file));
	}

	// generate texture
	textures[name].generate(width, height, data);

	// free image data
	stbi_image_free(data);
}

Texture2D& ResourceManager::getTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		std::cout << "Error ResourceManager: could not find the texture!\n";
	}

	return textures[name];
}

void ResourceManager::loadFont(const char* fontFilePath, const unsigned int fontSize, const std::string& name)
{
	// first clear the previously loaded Characters
	fonts[name].Characters.clear();

	// then initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		throw std::runtime_error("Could not init FreeType Library");
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, fontFilePath, 0, &face))
	{
		throw std::runtime_error(" Failed to load font: " + std::string(fontFilePath));
	}

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// then for the first 128 ASCII characters, pre-load/compile their characters and store them
	for (GLubyte c = 0; c < 128; c++) 
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			throw std::runtime_error("Failed to load Glyph");
			continue;
		}

		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};

		fonts[name].Characters.insert(std::pair<char, Character>(c, character));
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

Font& ResourceManager::getFont(const std::string& name)
{
	if (fonts.find(name) == fonts.end())
	{
		std::cout << "ERROR::RESOURCEMANAGER: Could not find the font!\n";
	}

	return fonts[name];
}

void ResourceManager::loadFlipbook(const char* directoryPath, const double& framesPerSecond, bool loop, const std::string& name)
{
	if (!std::filesystem::exists(directoryPath))
	{
		throw std::runtime_error("Cannot find directory: " + std::string(directoryPath));
	}

	if (!std::filesystem::is_directory(directoryPath))
	{
		throw std::runtime_error("The given path is not a directory: " + std::string(directoryPath));
	}

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		std::string path = entry.path().generic_string();
			
		ResourceManager::loadTexture(path.c_str(), true, path);
		flipbooks[name].addFrame(path);
	}

	flipbooks[name].setFramesPerSecond(framesPerSecond);
	flipbooks[name].setLoop(loop);
}

Flipbook& ResourceManager::getFlipbook(const std::string& name)
{
	if (flipbooks.find(name) == flipbooks.end())
	{
		std::cout << "ERROR::RESOURCEMANAGER: Could not find the flipbook!\n";
	}

	return flipbooks[name];
}

void ResourceManager::loadSound(const char* file, FMOD_MODE mode, const std::string& name)
{
	if (SoundManager::get().getSystem()->createSound(file, mode, nullptr, &sounds[name]) != FMOD_OK)
	{
		throw std::runtime_error("Failed to load sound: " + std::string(file));
	}
}

FMOD::Sound* ResourceManager::getSound(const std::string& name)
{
	if (sounds.find(name) == sounds.end())
	{
		std::cout << "ERROR::RESOURCEMANAGER: Could not find the sound!\n";
	}

	return sounds[name];
}

void ResourceManager::clear()
{
	// delete all shaders
	for (const auto& it : shaders)
	{
		glDeleteProgram(it.second.ID);
	}

	// delete all textures
	for (const auto& it : textures)
	{
		glDeleteTextures(1, &it.second.ID);
	}

	// delete all fonts
	for (auto& it : fonts)
	{
		it.second.Characters.clear();
	}
}

