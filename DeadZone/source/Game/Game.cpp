#include "Game.h"

#include <iostream> // TODO: debug
#include <fstream>

#include <nlohmann/json.hpp>

#include "../WindowManager/WindowManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/SpriteRenderer.h"
#include "../Renderer/TextRenderer.h"
#include "../GlobalClock/GlobalClock.h"
#include "../Map/Map.h"
#include "../Input/InputHandler.h"
#include "../Entity/Player/Player.h"
#include "../Entity/Enemy/Enemy.h"
#include "../Entity/AnimatedEntity.h"
#include "../Camera/Camera.h"
#include "../CollisionManager/CollisionManager.h"
#include "../InteractionManager/InteractionManager.h"
#include "../HUD/HUDManager.h"
#include "../MenuManager/MainMenu/MainMenu.h"
#include "../MenuManager/PauseMenu/PauseMenu.h"
#include "../SoundManager/SoundManager.h"
#include "../MenuManager/MenuManager.h"
#include "../WaveManager/WaveManager.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../Entity/Explosion/Explosion.h"

#include "../Client/Client.h"

Game::Game()
    : MAX_NUM_DEAD_BODIES(100) //daca sunt 100 de dead body-uri pe jos atunci incepem sa stergem in ordinea cronologica
{
    WindowManager::get();
    genUtil = GeneralUtilities::get();

    // TODO: mutat in main
    // Initialize ENet
    //if (enet_initialize() != 0)
    //{
    //    std::cout << "Error: An error occurred while initializing ENet" << std::endl;
    //}
    //atexit(enet_deinitialize);
}

Game::~Game()
{
    // cleanup
    Client::get().stop();
}

Game& Game::get()
{
    static Game instance;
    return instance;
}

void Game::loadResources()
{
    // Load JSON
    std::ifstream gameFile("config/game.json");
    nlohmann::json gameJSON;
    gameFile >> gameJSON;
    gameFile.close();

    // Start Client
    Client::get().start(gameJSON["serverAddress"].get<std::string>(), std::atoi(gameJSON["serverPort"].get<std::string>().c_str()), gameJSON["clientName"].get<std::string>());

    // Load Shaders
    try
    {
        for (auto& [shaderName, shader] : gameJSON["shaders"].items())
        {
            std::string* vertex = new std::string(shader["vertex"].get<std::string>());
            std::string* fragment = new std::string(shader["fragment"].get<std::string>());
            std::string* geometry = shader["geometry"].is_null() ? nullptr : new std::string(shader["geometry"].get<std::string>());
        
            ResourceManager::loadShader(vertex->c_str(), fragment->c_str(), geometry ? geometry->c_str() : nullptr, shaderName);

            delete vertex;
            delete fragment;
            delete geometry;
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::SHADER: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::SHADER: other error" << std::endl;
    }


    // Load Textures
    try
    {
        for (auto& [textureName, texture] : gameJSON["textures"].items())
        {
            std::string file = texture["file"].get<std::string>();
            bool alpha = texture["alpha"].get<bool>();

            ResourceManager::loadTexture(file.c_str(), alpha, textureName);
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::TEXTURE: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::TEXTURE: other error" << std::endl;
    }

    // Load Flipbooks
    try
    {
        for (auto& [flipbookName, flipbook] : gameJSON["flipbooks"].items())
        {
            std::string directory = flipbook["directory"].get<std::string>();
            double framesPerSecond = flipbook["framesPerSecond"].get<double>();
            bool loop = flipbook["loop"].get<bool>();

            ResourceManager::loadFlipbook(directory.c_str(), framesPerSecond, loop, flipbookName);
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::FLIPBOOK: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::FLIPBOOK: other error" << std::endl;
    }

    // Load Sounds
    try
    {
        for (auto& [soundName, sound] : gameJSON["sounds"].items())
        {
            std::string file = sound["file"].get<std::string>();
            int mode = sound["mode"].get<int>();

            ResourceManager::loadSound(file.c_str(), mode, soundName);
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::SOUND: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::SOUND: other error" << std::endl;
    }

    // Load Fonts
    try
    {
        for (auto& [fontName, font] : gameJSON["fonts"].items())
        {
            std::string file = font["file"].get<std::string>();
            int size = font["size"].get<int>();

            ResourceManager::loadFont(file.c_str(), size, fontName);
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::FONT: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::FONT: other error" << std::endl;
    }

    // Generate & Save and Load Map
    try
    {
        int width = gameJSON["map"]["width"].get<int>();
        int height = gameJSON["map"]["height"].get<int>();
        std::string file = this->generateProceduralMap(width, height);

        Map::get().readMap(file);
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "ERROR::MAP: " << err.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR::MAP: other error" << std::endl;
    }

    // Configure Shaders
    glm::mat4 projection = glm::ortho(-0.5f * static_cast<float>(WindowManager::get().getWindowWidth()), 0.5f * static_cast<float>(WindowManager::get().getWindowWidth()), -0.5f * static_cast<float>(WindowManager::get().getWindowHeight()), 0.5f * static_cast<float>(WindowManager::get().getWindowHeight()));
    ResourceManager::getShader("sprite").use().setInteger("sprite", 0);
    ResourceManager::getShader("sprite").use().setMatrix4("projection", projection);

    ResourceManager::getShader("player").use().setInteger("sprite", 0);
    ResourceManager::getShader("player").use().setMatrix4("projection", projection);

    // top-left coordinate of the scene will be at (0, 0) and the bottom-right part of the screen is at coordinate (WINDOW_WIDTH, WINDOW_HEIGHT)
    glm::mat4 orho = glm::ortho(0.0f, static_cast<float>(WindowManager::get().getWindowWidth()), static_cast<float>(WindowManager::get().getWindowHeight()), 0.0f);
    ResourceManager::getShader("text").use().setMatrix4("projection", orho);
    ResourceManager::getShader("text").use().setInteger("text", 0);
}

void Game::run()
{
    this->loadResources();

    Camera::get().setFollowsPlayer(true);

    // Setup Sound System
    SoundManager::get().play("walking", true);
    SoundManager::get().play("running", true);
    SoundManager::get().play("tired", true);
    SoundManager::get().play("soundtrack", true);

    // MainMenu::get().setupMainMenuInputComponent();
    MenuManager::get().push(MainMenu::get());

    // Setup Input
    InputHandler::setInputComponent(InputHandler::getMenuInputComponent());

    while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
    {
        if (gameStatus == GameStatus::InGame)
        {
            // Update
            Map::get().update();
            Camera::get().update();
            Player::get().update();
            this->updateEntities();


            // Collision System
            CollisionManager::get().handleCollisions(this->entities);

            // Interactions System
            InteractionManager::get().handleInteractions(this->entities);
        }


        // Client
        Client::get().update();



        // Render
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Map
        Map::get().draw();

        // Game Entities
        // Dead Bodies
        this->drawDeadBodies();

        // Player
        Player::get().draw();

        // Entities
        this->drawEntities();

        // HUD
        HUDManager::get().draw();

        if (gameStatus == GameStatus::InMenu)
        {
            // Main Menu
            try
            {
                MenuManager::get().play();
            }
            catch (noMenuOpened& err) {}
        }

        // Wave Manager
        if (MenuManager::get().size() == 0)
            WaveManager::get().update();

        // Update/Tick
        GlobalClock::get().updateTime();

        // Swap the screen buffers
        glfwSwapBuffers(WindowManager::get().getWindow());
       
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
    }
}

void Game::updateEntities()
{
	for (int i = 0; i < this->entitiesForNextFrame.size(); ++i)
		this->entities.emplace_back(this->entitiesForNextFrame[i]);
    this->entitiesForNextFrame.clear();

    for (int i = 0; i < this->entities.size(); ++i)
    {
        if (entities[i]->getDeleteEntity())
        {
            std::swap(entities[i], entities[entities.size() - 1]);
            entities.pop_back();
            --i;
        }
    }

    for (int i = 0; i < this->entities.size(); ++i)
        this->entities[i]->update();

    while (this->deadBodies.size() > this->MAX_NUM_DEAD_BODIES)
    {
        for (int i = 1; i < (int)this->deadBodies.size(); ++i)
        {
            this->deadBodies[i - 1] = this->deadBodies[i];
        }

        this->deadBodies.pop_back();
    }
}

void Game::drawDeadBodies()
{
    for (int i = 0; i < this->deadBodies.size(); ++i)
        this->deadBodies[i]->draw();
}

void Game::drawEntities() // grenazile si exploziile la urma (sunt mai la inaltime)
{
    for (int i = 0; i < this->entities.size(); ++i)
    {
        if (std::dynamic_pointer_cast<ThrownGrenade>(this->entities[i]))
            continue;
        if (std::dynamic_pointer_cast<Explosion>(this->entities[i]))
            continue;

        this->entities[i]->draw();
    }

    for (const auto& it : remotePlayers)
    {
        it.second->draw();
    }

    for (int i = 0; i < this->entities.size(); ++i) // grenazi
        if (std::dynamic_pointer_cast<ThrownGrenade>(this->entities[i]))
            this->entities[i]->draw();

    for (int i = 0; i < this->entities.size(); ++i) // explozii
        if (std::dynamic_pointer_cast<Explosion>(this->entities[i]))
            this->entities[i]->draw();
}

void Game::addEntityForNextFrame(std::shared_ptr<Entity> const entity)
{
    this->entitiesForNextFrame.emplace_back(entity);
}

void Game::addDeadBody(std::shared_ptr<DeadBody> const deadBody)
{
    this->deadBodies.emplace_back(deadBody);
}

void Game::addRemotePlayer(const std::string& clientKey, std::shared_ptr<RemotePlayer> const remotePlayer)
{
    this->remotePlayers[clientKey] = remotePlayer;
}

void Game::spawnRemotePlayer(const std::string& clientKey)
{
    if (remotePlayers.find(clientKey) != remotePlayers.end())
    {
        return;
    }

    addRemotePlayer(clientKey, std::make_shared<RemotePlayer>(10.5, 10.5, 1.0, 1.0, 0.0, 5.0, 0.4, 0.4, Player::ANIMATIONS_NAME_2D, Player::STATUSES, 7.5));
}

void Game::updateRemotePlayerPosition(const std::string& clientKey, double x, double y)
{
    if (remotePlayers.find(clientKey) == remotePlayers.end())
    {
        // TODO: delete std::cout
        std::cout << "ERROR: Could not find the remote player" << std::endl;
        return;
    }

    remotePlayers[clientKey]->setX(x);
    remotePlayers[clientKey]->setY(y);
}

void Game::putDoorsInEnclosedAreas(const int& width, const int& height, std::vector<std::vector<std::string>>& map, std::vector<std::vector<bool>>& enclosed) {
   std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, 0));
    const int di[4] = { -1, 0, 1, 0 };
    const int dj[4] = { 0, 1, 0, -1 };

    auto inside = [&](std::pair<int, int> cell) {
        if (cell.first < height && cell.first >= 0 && cell.second < width && cell.second >= 0)
            return 1;
        return 0;
        };

    std::queue<std::pair<int, int>> cellsInQueue;
    std::vector<std::pair<int, int>> candidatesForDoor;
    std::vector<std::pair<int, int>> visitedCells;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (map[i][j][0] == '.' && !visited[i][j]) {
                candidatesForDoor.clear();
                visitedCells.clear();
                int cnt = 0;
                cellsInQueue.push({ i, j });
                visited[i][j] = 1;
                visitedCells.push_back({ i, j });
                while (cellsInQueue.size() > 0) {
                    cnt++;
                    std::pair<int, int> cell = cellsInQueue.front();
                    cellsInQueue.pop();
                    for (int k = 0; k < 4; k++) {
                        std::pair<int, int> new_cell = { cell.first + di[k], cell.second + dj[k] };
                        if (inside(new_cell) && !visited[new_cell.first][new_cell.second]) {
                            if (map[new_cell.first][new_cell.second][0] == '.') {
                                cellsInQueue.push({ new_cell.first, new_cell.second });
                                visited[new_cell.first][new_cell.second] = 1;
                                visitedCells.push_back({ new_cell.first, new_cell.second });
                            }
                            else if (map[new_cell.first][new_cell.second][0] == 'M')
                                candidatesForDoor.push_back({ new_cell.first, new_cell.second });
                        }
                    }
                }
                if (cnt < 100) {
                    for (int k = 0; k < visitedCells.size(); k++) {
                        std::pair<int, int> cell = visitedCells[k];
                        enclosed[cell.first][cell.second] = 1;
                    }
                    for (int k = 0; k < candidatesForDoor.size(); k++) {
                        std::pair<int, int> cell = candidatesForDoor[k];
                        if (cell.first > 0 && cell.first < height - 1 && cell.second > 0 && cell.second < width - 1) {
                            map[cell.first][cell.second] = "D0";
                            break;
                        }
                    }
                }
            }
}

void Game::putShopInGoodArea(const int& width, const int& height, std::vector<std::vector<std::string>>& map, const std::vector<std::vector<bool>>& enclosed) {
    const std::vector<std::vector<std::string>> pattern = { {".2", ".6", ".1"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".8", ".9", ".7"}, {".4", ".5", ".3"} };

    std::vector<std::pair<int, int>> shopPrefered, shopAnyway;
    for (int i = 0; i < height - 6; i++)
        for (int j = 0; j < width - 3; j++) {
            bool okSP = 1, okSA = 1;
            for (int k = 0; k < 6; k++)
                for (int l = 0; l < 3; l++) {
                    if (map[i + k][j + l][0] != '.')
                        okSP = 0, okSA = 0;
                    if (enclosed[i + k][j + l] == 0)
                        okSP = 0;
                }
            if (okSP)
                shopPrefered.push_back({ i, j });
            if (okSA)
                shopAnyway.push_back({ i, j });
        }
    std::pair<int, int> positionForShop;
    if (shopPrefered.size() > 0) {
        int ind = Random::randomInt(1, static_cast<int>(shopAnyway.size())) - 1;
        positionForShop = shopPrefered[ind];
    }
    else {
        int ind = Random::randomInt(1, static_cast<int>(shopAnyway.size())) - 1;
        positionForShop = shopAnyway[ind];
    }
    std::cout << positionForShop.first << ' ' << positionForShop.second << '\n';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            map[i + positionForShop.first][j + positionForShop.second] = pattern[i][j];
    map[5 + positionForShop.first][1 + positionForShop.second] = "shop0";
}

std::string Game::generateProceduralMap(const int& width, const int& height) {

    std::vector<std::vector<std::string>> map(height, std::vector<std::string>(width, "."));

    // Generate Perlin Noise Map
    genUtil.generatePerlinMap(width, height, 10, 256, map);

    // Update corners
    for (int i = 0; i < height; i++)
        map[i][0] = map[i][width - 1] = "M0";

    for (int j = 0; j < width; j++)
        map[0][j] = map[height - 1][j] = "M0";

    // Put doors where areas are enclosed
    std::vector<std::vector<bool>> enclosed(height, std::vector<bool>(width, 0));
    putDoorsInEnclosedAreas(width, height, map, enclosed);

    // Find position for shop
    putShopInGoodArea(width, height, map, enclosed);

    // Gettime sinch epoch in ms
    long long ms_since_epoch = genUtil.getTimeSinceEpochInMs();

    std::string maps_dir = "maps/";
    std::string new_map_name = "sandbox_" + std::to_string(ms_since_epoch);
    std::string output_dir = maps_dir + new_map_name + ".map";

    // Output generated file
    std::ofstream MAP_OUTPUT(output_dir);
    for (int i = 0; i < height; i++, MAP_OUTPUT << "\n "[i == height])
        for (int j = 0; j < width; j++)
            MAP_OUTPUT << map[i][j] << ' ';

    return output_dir;
}
