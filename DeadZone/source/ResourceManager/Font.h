#pragma once

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

// holds all state information relevant to a character as loaded using FreeType
struct Character
{
	unsigned int textureID;	// ID handle of the glyph texture
	glm::ivec2 size;		// size of glyph
	glm::ivec2 bearing;		// offset from baseline to left/top of glyph
	unsigned int advance;	// horizontal offset to advance to next glyph
};

class Font
{
public:
	Font() = default;
	~Font() = default;

	Font(const Font& other) = default;
	Font& operator= (const Font& other) = default;

	// holds a list of pre-compiled Characters
	std::map<char, Character> Characters;
};

