#include "Game/Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <exception>

#include <enet/enet.h>
#include <nlohmann/json.hpp>

#include "Server/Server.h"

int main()
{
    // Initialize ENet
    if (enet_initialize() != 0)
    {
        std::cout << "Error: An error occurred while initializing ENet" << std::endl;
    }
    atexit(enet_deinitialize);

    // Load JSON
    std::ifstream gameFile("config/game.json");
    nlohmann::json gameJSON;
    gameFile >> gameJSON;
    gameFile.close();

    // TODO: doar test @Teodor
    // Start server
    if (gameJSON["clientHasServer"].get<bool>())
    {
        std::string serverPort = gameJSON["serverPort"].get<std::string>();

        std::thread serverThread([serverPort] {
            Server::get().start(serverPort);
            
            while (true) // TODO: ceva mecanism de stop
            {
                Server::get().update();
            }

            // TODO: delete
            Server::get().stop();
        });

        serverThread.detach();
    }

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

    // TODO: delete
    Server::get().stop();

	return 0;
}

