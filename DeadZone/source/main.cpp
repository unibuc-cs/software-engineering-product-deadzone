#include "Game/Game.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
	Game::get().run();

	return 0;
}

