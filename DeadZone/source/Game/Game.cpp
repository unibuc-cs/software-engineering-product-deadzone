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
#include "../MenuManager/EndScreen/EndScreen.h"
#include "../MenuManager/ShopMenu/ShopMenu.h"
#include "../SoundManager/SoundManager.h"
#include "../MenuManager/MenuManager.h"
#include "../WaveManager/WaveManager.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../Entity/Explosion/Explosion.h"

#include "../Client/Client.h"
#include "../Server/Server.h"

Game::Game()
    : MAX_NUM_DEAD_BODIES(100) // daca sunt 100 de dead body-uri pe jos atunci incepem sa stergem in ordinea cronologica
	, isServer(false), isInMatch(false), hasGameMode(false), isServerRunning(false)
{
    WindowManager::get();

    // Create "save.json" file if it doesn't exist
    std::ifstream readSaveFile("config/save.json");
    if (!readSaveFile.is_open())
    {
        nlohmann::json saveJSON = nlohmann::json::object();
        std::ofstream writeSaveFile("config/save.json");
        writeSaveFile << saveJSON.dump(4) << std::endl;
        writeSaveFile.close();
    }
    else
    {
        readSaveFile.close();
    }
}

Game::~Game()
{

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
    SoundManager::get().play("soundtrack", true, false);

    // MainMenu::get().setupMainMenuInputComponent();
    MenuManager::get().push(MainMenu::get());

    // Setup Input
    InputHandler::setInputComponent(InputHandler::getMenuInputComponent());

    while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
    {
        // Update/Tick
        GlobalClock::get().updateTime();

        if (isInMatch)
        {
            // Serverul e pe un thread separat

            // Client
            Client::get().update();

            // wait
            if (!Map::get().getHasBeenLoaded() && !getHasGameMode())
            {
                Client::get().update();
                continue;
            }
        }
        
        if (gameStatus == GameStatus::InGame 
            || dynamic_cast<PauseMenu*>(&MenuManager::get().top())
            || dynamic_cast<EndScreen*>(&MenuManager::get().top())
            || dynamic_cast<ShopMenuAbstract*>(&MenuManager::get().top()))
        {
            // Update
            Map::get().update();
            Camera::get().update();
            Player::get().update();
            this->updateEntities();

            // Collision System
            CollisionManager::get().handleCollisions(this->entities);
            CollisionManager::get().handleMultiplayerCollisions(this->entities);

            // Interactions System
            InteractionManager::get().handleInteractions(this->entities);
        }

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

        // Wave Manager
        if ((MenuManager::get().size() == 0
            || dynamic_cast<PauseMenu*>(&MenuManager::get().top())
            || dynamic_cast<EndScreen*>(&MenuManager::get().top())
            || dynamic_cast<ShopMenuAbstract*>(&MenuManager::get().top()))
            && gameMode == GameMode::Survival)
        {
            WaveManager::get().update();
            WaveManager::get().draw();
        }

        // Menu manager
        if (gameStatus == GameStatus::InMenu)
        {
            // Main Menu
            try
            {
                MenuManager::get().play();
            }
            catch (noMenuOpened& err) {}
        }

        if (Client::get().getShouldDisconnect())
        {
            Game::get().stopConnection();

            Game::get().clear();
            Game::get().setIsInMatch(false);
            Game::get().setHasGameMode(false);

            WaveManager::deleteInstance();
            Player::deleteInstance();
            Map::deleteInstance();

            MenuManager::get().clear();

            MenuManager::get().push(MainMenu::get());
            InputHandler::setInputComponent(InputHandler::getMenuInputComponent());
        }

        // Swap the screen buffers
        glfwSwapBuffers(WindowManager::get().getWindow());
       
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
    }

    // Stop Multiplayer
    Game::stopConnection();
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
        // TODO: throw error
        return;
    }
    addRemotePlayer(clientKey, std::make_shared<RemotePlayer>(10.5, 10.5, 1.0, 1.0, 0.0, 5.0, 0.4, 0.4, Player::ANIMATIONS_NAME_2D, Player::STATUSES, 7.5));
}

void Game::removeRemotePlayer(const std::string& clientKey)
{
    remotePlayers.erase(clientKey);
}

void Game::updateRemotePlayerClientName(const std::string& clientKey, const std::string& name)
{
    remotePlayers[clientKey]->setClientName(name);
}

void Game::updateRemotePlayerTeam(const std::string& clientKey, int newTeam)
{
    remotePlayers[clientKey]->setTeam(newTeam);
}

void Game::updateRemotePlayerOutfitColor(const std::string& clientKey, const glm::vec3& color)
{
    remotePlayers[clientKey]->setOutfitColor(color);
}

void Game::updateRemotePlayerPosition(const std::string& clientKey, double x, double y)
{
    remotePlayers[clientKey]->setX(x);
    remotePlayers[clientKey]->setY(y);
}

void Game::updateRemotePlayerRotateAngle(const std::string& clientKey, double angle)
{
    remotePlayers[clientKey]->setRotateAngle(angle);
}

void Game::updateRemotePlayerStatuses(const std::string& clientKey, const std::vector<AnimatedEntity::EntityStatus>& statuses)
{
    for (size_t indexStatus = 0; indexStatus < statuses.size(); ++indexStatus)
    {
        remotePlayers[clientKey]->updateStatus(statuses[indexStatus], indexStatus);
    }
}

void Game::applyRemotePlayerCloseRangeDamage(const std::string& clientKey, double damage, double shortRangeAttackRadius)
{
    // entities
    std::vector<std::shared_ptr<Entity>>& entities = Game::get().getEntities();
    for (int i = 0; i < entities.size(); ++i)
    {
        const bool enemyInRange = Weapon::applyCloseRangeDamage(
            glm::vec2(remotePlayers[clientKey]->getX(), remotePlayers[clientKey]->getY()),
            remotePlayers[clientKey]->getRotateAngle(),
            glm::vec2(entities[i]->getX(), entities[i]->getY()),
            shortRangeAttackRadius
        );

        if (std::dynamic_pointer_cast<Human>(entities[i]) && enemyInRange)
        {
            std::dynamic_pointer_cast<Human>(entities[i])->setHealth(std::max(0.0, std::dynamic_pointer_cast<Human>(entities[i])->getHealth() - damage));
        }
    }

    // remoteZombies
    for (auto& [remoteZombieId, remoteZombie] : WaveManager::get().getRemoteZombies())
    {
        const bool enemyInRange = Weapon::applyCloseRangeDamage(
            glm::vec2(remotePlayers[clientKey]->getX(), remotePlayers[clientKey]->getY()),
            remotePlayers[clientKey]->getRotateAngle(),
            glm::vec2(remoteZombie->getX(), remoteZombie->getY()),
            shortRangeAttackRadius
        );

        if (enemyInRange)
        {
            remoteZombie->setHealth(std::max(0.0, remoteZombie->getHealth() - damage));
        }
    }

    // Player
    const bool enemyInRange = Weapon::applyCloseRangeDamage(
        glm::vec2(remotePlayers[clientKey]->getX(), remotePlayers[clientKey]->getY()),
        remotePlayers[clientKey]->getRotateAngle(),
        glm::vec2(Player::get().getX(), Player::get().getY()),
        shortRangeAttackRadius
    );

    if (enemyInRange 
        && (gameMode == GameMode::Survival 
            || Player::get().getTeam() != getRemotePlayerTeam(clientKey)))
    {
        bool notDeadBefore = !Player::get().isDead();

        Player::get().applyDamage(damage);

        if (Player::get().isDead() && notDeadBefore)
        {
            Client::get().sendConfirmedKill(clientKey);
        }
    }
}

int Game::getRemotePlayerTeam(const std::string& clientKey)
{
    if (remotePlayers.find(clientKey) == remotePlayers.end())
    {
        std::cout << "Could not find the remote player - " << clientKey << std::endl;
        return -1;
    }

    return remotePlayers[clientKey]->getTeam();
}

void Game::establishConnection()
{
    // Load JSON
    std::ifstream saveFile("config/save.json");
    nlohmann::json saveJSON;
    saveFile >> saveJSON;
    saveFile.close();

    // Start server
    isServer = saveJSON["clientHasServer"].get<bool>();
    if (isServer)
    {
        std::string serverPort = saveJSON["createServerPort"].get<std::string>();

        //Server::get().start(serverPort);

		// Game::stopConnection(); // nu ar trebui sa fie necesar

		this->isServerRunning = true;
		this->serverThread = std::make_shared<std::thread>([this, serverPort]()
		{
			Server::get().start(serverPort);

			this->isServerRunningMutex.lock();
			while (this->isServerRunning)
			{
				this->isServerRunningMutex.unlock();

				Server::get().update();

				this->isServerRunningMutex.lock();
			}
			this->isServerRunningMutex.unlock();

			Server::get().stop();
		});
    }

    // Start Client
    if (isServer)
    {
        Client::get().start("localhost", std::atoi(saveJSON["createServerPort"].get<std::string>().c_str()), saveJSON["clientName"].get<std::string>());
    }
    else
    {
        Client::get().start(saveJSON["joinServerAddress"].get<std::string>(), std::atoi(saveJSON["joinServerPort"].get<std::string>().c_str()), saveJSON["clientName"].get<std::string>());
    }
}

void Game::stopConnection()
{
    Client::get().sendDisconnect();
	Client::get().stop();

	if (this->serverThread)
	{
		this->isServerRunningMutex.lock();
		this->isServerRunning = false;
		this->isServerRunningMutex.unlock();

		this->serverThread->join();
		this->serverThread = nullptr;

        Server::get().stop();
	}
}
