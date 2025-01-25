#include "Game/Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <exception>

#include <enet/enet.h>

#include "Server/Server.h"

int main()
{
    // Initialize ENet
    if (enet_initialize() != 0)
    {
        std::cout << "Error: An error occurred while initializing ENet" << std::endl;
    }
    atexit(enet_deinitialize);

    try
    {
	    Game::get().run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unexpected error" << std::endl;
    }

	return 0;
}

