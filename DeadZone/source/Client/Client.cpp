#include "Client.h"

#include <nlohmann/json.hpp>

#include <iostream>

#include "../GlobalClock/GlobalClock.h"
#include "../Game/Game.h"
#include "../Entity/Player/Player.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../WaveManager/WaveManager.h"
#include "../Map/Map.h"
#include "../SoundManager/SoundManager.h"

Client::Client()
	: MAX_NUM_SERVERS(1), NUM_CHANNELS(1), TIME_WAITING_FOR_EVENTS_MS(0) // TODO: test ca sa proceseze mai rpd
	, serverPeer(nullptr), client(NULL), serverAddress(), eNetEvent()
	, succesfullyConnected(false)
	, lastTimeTriedConnection(0.0f)
	, RETRY_CONNECTION_DELTA_TIME(1.0f)
	, TIME_BETWEEN_PINGS(10000.0f) // TODO: test ca sa nu mai trimit prea multe request-uri
	, MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST(500000.0f) // TODO: test ca sa nu mai trimit prea multe request-uri
	, lastTimeReceivedPing(0.0f)
	, lastTimeSentPing(0.0f)
	, clientName("")
	, workingServerConnection(false)
	, lastRemotePlayerData(10.5, 10.5, 1.0, 1.0, 0.0, 5.0, 0.4, 0.4, Player::ANIMATIONS_NAME_2D, Player::STATUSES, 7.5)
{

}

Client::~Client()
{

}

Client& Client::get()
{
	static Client instance;
	return instance;
}

void Client::start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName)
{
	this->stop();

	this->clientName = clientName;

	// init enet client
	this->client = enet_host_create(NULL, this->MAX_NUM_SERVERS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite la latimea de banda
	if (client == NULL)
	{
		std::cout << "Error: ENet failed to create client" << std::endl;
	}

	enet_address_set_host(&this->serverAddress, serverIP.c_str());
	this->serverAddress.port = serverPort;

	std::cout << "Client initialized with: " << serverIP << ' ' << this->serverAddress.port << ' ' << this->clientName << ' ' << std::endl;
}

void Client::sendMessage(const std::string& messageToSend, bool& failedToSendMessage, float& timeWhenMessageSent)
{
	if (!this->succesfullyConnected)
	{
		std::cout << "Error: Client cannot send message because it is not connected to server" << std::endl;
		return;
	}

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->serverPeer, 0, packet) == 0)
	{
		timeWhenMessageSent = GlobalClock::get().getCurrentTime();
		failedToSendMessage = false;

		// TODO: uncomment
		// std::cout << "Client sent message: " << messageToSend << std::endl;
	}
	else
	{
		failedToSendMessage = true;
		std::cout << "Error: Client failed to send message" << std::endl;
	}
}

void Client::sendMessageUnsafe(const std::string& messageToSend, float& timeWhenMessageSent)
{
	if (!this->succesfullyConnected)
	{
		std::cout << "Error: Client cannot send message because it is not connected to server" << std::endl;
		return;
	}

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->serverPeer, 0, packet) == 0)
	{
		timeWhenMessageSent = GlobalClock::get().getCurrentTime();

		// TODO: uncomment
		// std::cout << "Client sent message: " << messageToSend << std::endl;
	}
	else
	{
		std::cout << "Error: Client failed to send message" << std::endl;
	}
}

bool Client::shouldSendRemotePlayerData()
{
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		return true;
	}

	bool shouldSend = false;

	if (Player::get().getX() != lastRemotePlayerData.getX())
	{
		lastRemotePlayerData.setX(Player::get().getX());
		shouldSend = true;
	}
	if (Player::get().getY() != lastRemotePlayerData.getY())
	{
		lastRemotePlayerData.setY(Player::get().getY());
		shouldSend = true;
	}

	if (Player::get().getRotateAngle() != lastRemotePlayerData.getRotateAngle())
	{
		lastRemotePlayerData.setRotateAngle(Player::get().getRotateAngle());
		shouldSend = true;
	}

	for (size_t indexStatus = 0; indexStatus < Player::get().getStatuses().size(); ++indexStatus)
	{
		if (Player::get().getStatus(indexStatus) != lastRemotePlayerData.getStatus(indexStatus))
		{
			lastRemotePlayerData.updateStatus(Player::get().getStatus(indexStatus), indexStatus);
			shouldSend = true;
		}
	}

	return shouldSend;
}

void Client::handleReceivedPacket()
{
	if (this->eNetEvent.packet->dataLength == 0)
	{
		std::cout << "Warning: Client received empty packet" << std::endl;
		return;
	}

	this->lastTimeReceivedPing = GlobalClock::get().getCurrentTime();

	std::string receivedMessage((char*)this->eNetEvent.packet->data);
	// TODO: uncomment
	// std::cout << "CLIENT: Received Message from server: " << receivedMessage << std::endl;

	// parse json input data
	nlohmann::json jsonData = nlohmann::json::parse(receivedMessage);

	if (jsonData.contains("remotePlayers"))
	{
		for (const auto& [clientKey, playerData] : jsonData["remotePlayers"].items())
		{
			// asigurare ca avem spawned remote player-ul pe care vrem sa il actualizam
			Game::get().spawnRemotePlayer(clientKey);

			// TODO: deocamdata trimit toate atributele despre player => trimite doar ce e nou

			// clientName
			Game::get().updateRemotePlayerClientName(clientKey, playerData["clientName"].get<std::string>());

			// outfitColor
			glm::vec3 outfitColor = glm::vec3(
				playerData["outfitColor"]["x"].get<double>(),
				playerData["outfitColor"]["y"].get<double>(),
				playerData["outfitColor"]["z"].get<double>()
			);
			Game::get().updateRemotePlayerOutfitColor(clientKey, outfitColor);

			// position
			Game::get().updateRemotePlayerPosition(clientKey, playerData["position"]["x"].get<double>(), playerData["position"]["y"].get<double>());

			// rotateAngle
			Game::get().updateRemotePlayerRotateAngle(clientKey, playerData["rotateAngle"].get<double>());
			
			// statuses
			std::vector<AnimatedEntity::EntityStatus> statuses;
			for (const auto& status : playerData["statuses"])
			{
				statuses.push_back(static_cast<AnimatedEntity::EntityStatus>(status.get<int>()));
			}
			Game::get().updateRemotePlayerStatuses(clientKey, statuses);
		}
	}

	// bullet
	if (jsonData.contains("bullets"))
	{
		for (const auto& [clientKey, bulletData] : jsonData["bullets"].items())
		{
			bool isThrownGrenade = bulletData["isThrownGrenade"].get<bool>();
			if (isThrownGrenade)
			{
				Game::get().addEntityForNextFrame(std::make_shared<ThrownGrenade>(
					bulletData["x"].get<double>(), bulletData["y"].get<double>(),
					0.3, 0.3,
					bulletData["rotateAngle"].get<double>(),
					bulletData["speed"].get<double>(),
					0.3, 0.3,
					bulletData["textureName2D"].get<std::string>(),
					0.0,
					1.0,
					bulletData["damage"].get<double>(),
					15.0,
					1.0
				));
			}
			else
			{
				Game::get().addEntityForNextFrame(std::make_shared<Bullet>(
					bulletData["x"].get<double>(), bulletData["y"].get<double>(),
					0.3, 0.3,
					bulletData["rotateAngle"].get<double>(),
					bulletData["speed"].get<double>(),
					0.3, 0.3,
					bulletData["textureName2D"].get<std::string>(),
					bulletData["damage"].get<double>()
				));
			}
		}
	}

	// closeRangeDamages
	if (jsonData.contains("closeRangeDamages"))
	{
		for (const auto& [clientKey, closeRangeDamageData] : jsonData["closeRangeDamages"].items())
		{
			Game::get().applyRemotePlayerCloseRangeDamage(
				clientKey,
				closeRangeDamageData["damage"],
				closeRangeDamageData["shortRangeAttackRadius"]
			);
		}
	}

	// sounds
	if (jsonData.contains("sounds"))
	{
		for (const auto& [clientKey, soundData] : jsonData["sounds"].items())
		{
			SoundManager::get().play(
				soundData["name"].get<std::string>(),
				soundData["paused"].get<bool>(),
				false
			);
		}
	}

	// zombies
	if (jsonData.contains("zombies") && !Game::get().getIsServer()) // TODO: skip server
	{
		for (const auto& [zombieId, zombieData] : jsonData["zombies"].items())
		{
			// asigurare ca avem spawned remote zombie-ul pe care vrem sa il actualizam
			WaveManager::get().spawnRemoteZombie(zombieId, zombieData["position"]["x"].get<double>(), zombieData["position"]["y"].get<double>());

			WaveManager::get().updateRemoteZombiePosition(zombieId, zombieData["position"]["x"].get<double>(), zombieData["position"]["y"].get<double>());
			WaveManager::get().updateRemoteZombieRotateAngle(zombieId, zombieData["rotateAngle"].get<double>());

			std::vector<AnimatedEntity::EntityStatus> statuses;
			for (const auto& status : zombieData["statuses"])
			{
				statuses.push_back(static_cast<AnimatedEntity::EntityStatus>(status.get<int>()));
			}
			WaveManager::get().updateRemoteZombieStatuses(zombieId, statuses);

			WaveManager::get().updateRemoteZombieDeleteEntity(zombieId, zombieData["deleteEntity"].get<bool>());
		}
	}

	// openedDoors
	if (jsonData.contains("openedDoors"))
	{
		for (const auto& [clientKey, openedDoorData] : jsonData["openedDoors"].items())
		{
			Map::get().updateDoorStatus(openedDoorData["id"].get<unsigned int>());
		}
	}

	// map
	if (jsonData.contains("map") && !Map::get().getHasBeenLoaded())
	{
		Map::get().readMapFromBuffer(jsonData["map"].get<std::vector<std::vector<std::string>>>());
	}

	// game mode
	if (jsonData.contains("gameMode") && !Game::get().getHasGameMode())
	{
		Game::get().setGameMode(static_cast<Game::GameMode>(jsonData["gameMode"].get<unsigned int>()));
		Game::get().setHasGameMode(true);
	}

	// waveNumber
	if (jsonData.contains("waveNumber"))
	{
		WaveManager::get().setNumFinishedWaves(jsonData["waveNumber"].get<int>());
	}

	// disconnectClient
	if (jsonData.contains("disconnectClient"))
	{
		std::cout << "CLIENT: disconnect " << jsonData["disconnectClient"].get<std::string>() << std::endl;
		Game::get().removeRemotePlayer(jsonData["disconnectClient"].get<std::string>());
	}

	enet_packet_destroy(this->eNetEvent.packet);
}

void Client::update()
{
	if (!this->succesfullyConnected)
	{
		if (GlobalClock::get().getCurrentTime() - this->lastTimeTriedConnection < this->RETRY_CONNECTION_DELTA_TIME)
			return;

		this->serverPeer = enet_host_connect(this->client, &this->serverAddress, this->NUM_CHANNELS, 0); // 0 = nu trimitem nimic
		if (this->serverPeer == NULL)
		{
			std::cout << "Error: No available peers for initiating an ENet connection (no server available)" << std::endl;
		}
		else
		{
			this->succesfullyConnected = true;
			std::cout << "Client connected to server" << std::endl;
		}

		return;
	}

	// map
	if (!Map::get().getHasBeenLoaded())
	{
		nlohmann::json jsonData;
		jsonData["map"] = true;

		this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
	}

	// game mode
	if (!Game::get().getHasGameMode())
	{
		nlohmann::json jsonData;
		jsonData["gameMode"] = true;

		this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
	}

	// Trimitem ce informatii vitale stim deja catre server.
	if (shouldSendRemotePlayerData())
	{
		nlohmann::json jsonData;

		// TODO: trimite doar ce avem de schimbat

		jsonData["clientName"] = this->clientName;
		jsonData["outfitColor"]["x"] = Player::get().getOutfitColor().x;
		jsonData["outfitColor"]["y"] = Player::get().getOutfitColor().y;
		jsonData["outfitColor"]["z"] = Player::get().getOutfitColor().z;
		jsonData["position"]["x"] = Player::get().getX();
		jsonData["position"]["y"] = Player::get().getY();
		jsonData["rotateAngle"] = Player::get().getRotateAngle();
		jsonData["statuses"] = Player::get().getStatuses();

		// TODO: isWalking
		// TODO: isRunning
		// TODO: isDead

		sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
	}

	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->client, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
	while (code != 0)
	{
		if (code > 0)
		{
			switch (this->eNetEvent.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				this->handleReceivedPacket();
				break;
			default:
				std::cout << "Warning: Client received unrecognized event type" << std::endl;
				break;
			}
		}
		else if (code < 0)
		{
			std::cout << "Error: Client service failed" << std::endl;
			break;
		}

		code = enet_host_service(this->client, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
	}

	// Vedem daca am pierdut conexiunea cu serverul.
	if (GlobalClock::get().getCurrentTime() - this->lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
	{
		this->workingServerConnection = false;
	}
	else
	{
		this->workingServerConnection = true;
	}

	// Apoi trimitem ping-ul catre server.
	if (GlobalClock::get().getCurrentTime() - this->lastTimeSentPing > this->TIME_BETWEEN_PINGS)
	{
		nlohmann::json jsonData;
		jsonData["ping"] = true;
		jsonData["clientName"] = this->clientName;

		this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
	}
}

void Client::stop()
{
	if (this->serverPeer != nullptr)
		enet_peer_disconnect(this->serverPeer, 0);
	if (this->client != nullptr)
		enet_host_destroy(this->client);

	this->serverPeer = nullptr;
	this->client = nullptr;

	this->serverAddress = ENetAddress();
	this->eNetEvent = ENetEvent();


	this->succesfullyConnected = false;
	this->lastTimeTriedConnection = 0.0f;

	this->lastTimeReceivedPing = 0.0f;
	this->lastTimeSentPing = 0.0f;

	this->clientName = "";

	this->workingServerConnection = false;
}

void Client::sendBullet(const std::shared_ptr<Bullet>& const entity)
{
	bool isThrownGrenade = false;
	if (std::dynamic_pointer_cast<ThrownGrenade>(entity))
	{
		isThrownGrenade = true;
	}

	nlohmann::json jsonData;

	jsonData["bullet"]["isThrownGrenade"] = isThrownGrenade;
	jsonData["bullet"]["x"] = entity->getX();
	jsonData["bullet"]["y"] = entity->getY();
	jsonData["bullet"]["rotateAngle"] = entity->getRotateAngle();
	jsonData["bullet"]["speed"] = entity->getSpeed();
	jsonData["bullet"]["textureName2D"] = entity->getTextureName2D();
	jsonData["bullet"]["damage"] = entity->getDamage();

	// TODO: trimite si ceilalti parametrii daca vrem sa avem mai multi modificatori

	this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
}

void Client::sendSound(const std::string& name, bool paused)
{
	nlohmann::json jsonData;

	jsonData["sound"]["name"] = name;
	jsonData["sound"]["paused"] = paused;

	this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
}

void Client::sendOpenedDoor(int id)
{
	nlohmann::json jsonData;

	jsonData["openedDoor"] = id;

	this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
}

void Client::sendCloseRangeDamage(const double damage, const double shortRangeAttackRadius)
{
	nlohmann::json jsonData;

	jsonData["closeRangeDamage"]["damage"] = damage;
	jsonData["closeRangeDamage"]["shortRangeAttackRadius"] = shortRangeAttackRadius;

	this->sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
}

void Client::sendDisconnect()
{
	nlohmann::json jsonData;
	jsonData["disconnect"] = true;

	bool failure = true;
	while (failure)
	{
		this->sendMessage(jsonData.dump(), failure, this->lastTimeSentPing);
	}

	enet_host_flush(this->client);
}
