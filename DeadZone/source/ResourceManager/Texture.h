#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
	Texture2D();
	~Texture2D() = default;

	Texture2D(const Texture2D& other) = default;
	Texture2D& operator= (const Texture2D& other) = default;

	// holds the ID of the texture object
	unsigned int ID;

	// texture image dimensions
	unsigned int width, height;		// width and height of loaded image in pixels

	// texture format
	unsigned int internalFormat;	// format of texture object
	unsigned int imageFormat;		// format of loaded image

	// texture configuration
	unsigned int wrap_S;			// wrapping mode for S axis
	unsigned int wrap_T;			// wrapping mode for T axis
	unsigned int min_filter;		// filtering mode if texture pixels < screen pixels
	unsigned int max_filter;		// filtering mode if texture pixels > screen pixels

	// generate texture from image data
	void generate(unsigned int width, unsigned int height, unsigned char* data);

	// bind the texture as the current active GL_TEXTURE_2D texture object
	void bind() const;
};

