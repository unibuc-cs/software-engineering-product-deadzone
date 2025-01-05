#include "Server.h"

#include <nlohmann/json.hpp>

#include "../Random/Random.h"
#include "../GlobalClock/GlobalClock.h"
#include "../Entity/Bullet/ThrownGrenade.h"
#include "../Map/Map.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>


ReplicatedSound::ReplicatedSound(const std::string& name, bool paused)
	: name(name)
	, paused(paused)
{

}

Server::Server()
	: MAX_NUM_CLIENTS((1 << 5)), NUM_CHANNELS(2), TIME_WAITING_FOR_EVENTS_MS(0) // TODO: test ca sa proceseze mai rpd
	, server(nullptr), address(), MINIMUM_PORT(10000), MAXIMUM_PORT(20000)
	, eNetEvent()
	, succesfullyCreated(false), lastTimeTriedCreation(0.0f), RETRY_CREATION_DELTA_TIME(1.0f)
	, TIME_BETWEEN_PINGS(10000.0f), MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST(500000.0f) // TODO: test ca sa nu mai trimit prea multe request-uri
	, updateClients(false)
{
	this->address.host = ENET_HOST_ANY;
	this->address.port = 0;

	generateMap();
}

Server::~Server()
{

}

Server& Server::get()
{
	static Server instance;
	return instance;
}

void Server::start()
{
	this->stop();

	this->address.port = Random::randomInt(this->MINIMUM_PORT, this->MAXIMUM_PORT);
}

void Server::start(const std::string& serverPort)
{
	this->stop();

	this->address.port = std::stoi(serverPort);
}

void Server::ClientData::sendMessage(const std::string& messageToSend, bool& failedToSendMessage)
{
	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->peer, 0, packet) == 0)
	{
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
		failedToSendMessage = false;
	}
	else
	{
		failedToSendMessage = true;
		std::cout << "Error: Client failed to send message" << std::endl;
	}
}

void Server::ClientData::sendMessageUnsafe(const std::string& messageToSend)
{
	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->peer, 0, packet) == 0)
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
	else
		std::cout << "Error: Client failed to send message" << std::endl;
}

void Server::handleReceivedPacket()
{
	std::string clientKey = this->getClientKey(this->eNetEvent.peer->address);

	if (this->eNetEvent.packet->dataLength == 0)
	{
		std::cout << "Warning: Server received empty packet" << std::endl;
		return;
	}

	// verifica limita maxima de clienti
	if (this->connectedClients.size() == this->MAX_NUM_CLIENTS
		&& this->connectedClients.find(clientKey) == this->connectedClients.end())
	{
		std::cout << "Warning: Server reached maximum number of clients and could not allow a recently arrived client to connect" << std::endl;
		return;
	}

	// adauga un client nou
	if (this->connectedClients.find(clientKey) == this->connectedClients.end())
	{
		this->connectedClients.insert({
			clientKey, ClientData()
			});
		this->connectedClients.find(clientKey)->second.peer = this->eNetEvent.peer;
	}
	this->connectedClients.find(clientKey)->second.lastTimeReceivedPing = GlobalClock::get().getCurrentTime();

	// parse json input data
	std::string receivedMessage((char*)this->eNetEvent.packet->data);
	
	// TODO: uncomment
	// std::cout << "SERVER: Received Message from " << this->connectedClients.find(clientKey)->second.clientName << ": " << receivedMessage << std::endl;

	nlohmann::json jsonData = nlohmann::json::parse(receivedMessage);

	// player
	if (jsonData.contains("ping"))
	{
		connectedClients[clientKey].lastTimeReceivedPing = GlobalClock::get().getCurrentTime();
	}
	if (jsonData.contains("clientName"))
	{
		connectedClients[clientKey].remotePlayerData.setClientName(jsonData["clientName"].get<std::string>());
	}
	if (jsonData.contains("outfitColor"))
	{
		glm::vec3 outfitColor = glm::vec3(
			jsonData["outfitColor"]["x"].get<double>(),
			jsonData["outfitColor"]["y"].get<double>(),
			jsonData["outfitColor"]["z"].get<double>()
		);
		connectedClients[clientKey].remotePlayerData.setOutfitColor(outfitColor);
	}
	if (jsonData.contains("position"))
	{
		connectedClients[clientKey].remotePlayerData.setX(jsonData["position"]["x"].get<double>());
		connectedClients[clientKey].remotePlayerData.setY(jsonData["position"]["y"].get<double>());
	}
	if (jsonData.contains("rotateAngle"))
	{
		connectedClients[clientKey].remotePlayerData.setRotateAngle(jsonData["rotateAngle"].get<double>());
	}
	if (jsonData.contains("statuses"))
	{
		int indexStatus = 0;
		for (const auto& status : jsonData["statuses"])
		{
			connectedClients[clientKey].remotePlayerData.updateStatus(static_cast<AnimatedEntity::EntityStatus>(status.get<int>()), indexStatus++);
		}
	}

	// bullet
	if (jsonData.contains("bullet"))
	{
		bool isThrownGrenade = jsonData["bullet"]["isThrownGrenade"].get<bool>();
		if (isThrownGrenade)
		{
			connectedClients[clientKey].bulletData = std::make_shared<ThrownGrenade>(
				jsonData["bullet"]["x"].get<double>(), jsonData["bullet"]["y"].get<double>(),
				0.3, 0.3,
				jsonData["bullet"]["rotateAngle"].get<double>(),
				jsonData["bullet"]["speed"].get<double>(),
				0.3, 0.3,
				jsonData["bullet"]["textureName2D"].get<std::string>(),
				0.0,
				1.0,
				jsonData["bullet"]["damage"].get<double>(),
				15.0,
				1.0
			);
		}
		else
		{
			connectedClients[clientKey].bulletData = std::make_shared<Bullet>(
				jsonData["bullet"]["x"].get<double>(), jsonData["bullet"]["y"].get<double>(),
				0.3, 0.3,
				jsonData["bullet"]["rotateAngle"].get<double>(),
				jsonData["bullet"]["speed"].get<double>(),
				0.3, 0.3,
				jsonData["bullet"]["textureName2D"].get<std::string>(),
				jsonData["bullet"]["damage"].get<double>()
			);
		}
	}

	// sound
	if (jsonData.contains("sound"))
	{
		connectedClients[clientKey].soundData = std::make_shared<ReplicatedSound>(
			jsonData["sound"]["name"].get<std::string>(),
			jsonData["sound"]["paused"].get<bool>()
		);
	}

	// openedDoor
	if (jsonData.contains("openedDoor"))
	{
		connectedClients[clientKey].openedDoorData = std::make_shared<unsigned int>(jsonData["openedDoor"].get<unsigned int>());
	}

	// map
	if (jsonData.contains("map"))
	{
		// trimite map catre clientul nou
		sendMap(clientKey);
	}

	// TODO: de pus in if-uri? excluzand "ping"
	updateClients = true;

	enet_packet_destroy(this->eNetEvent.packet);
}

void Server::generateMap()
{
	// Load JSON
	std::ifstream gameFile("config/game.json");
	nlohmann::json gameJSON;
	gameFile >> gameJSON;
	gameFile.close();

	int width = gameJSON["map"]["width"].get<int>();
	int height = gameJSON["map"]["height"].get<int>();

	// TODO: foloseste mapa generata procedural
	std::string filePath = Map::generateProceduralMap(width, height);
	// std::string filePath = "maps/sandbox.map";

	std::ios_base::sync_with_stdio(false);

	std::ifstream in(filePath);
	if (in.fail())
	{
		throw std::runtime_error("Cannot open file: " + filePath);
	}
	in.tie(nullptr);

	while (!in.eof())
	{
		std::string line;
		std::getline(in, line);
		std::stringstream ss(line);

		map.push_back(std::vector<std::string>());

		std::string code;
		while (ss >> code)
		{
			map.back().push_back(code);
		}
	}

	in.close();
}

void Server::update()
{
	if (!this->succesfullyCreated)
	{
		if (GlobalClock::get().getCurrentTime() - this->lastTimeTriedCreation < this->RETRY_CREATION_DELTA_TIME)
			return;

		this->lastTimeTriedCreation = GlobalClock::get().getCurrentTime();
		this->server = enet_host_create(&this->address, this->MAX_NUM_CLIENTS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite pe latimea de banda

		if (this->server == nullptr)
		{
			std::cout << "Error: ENet server host creation failed" << std::endl;
		}
		else
		{
			this->succesfullyCreated = true;
			std::cout << "Server started on port " << this->address.port << std::endl;
		}

		return;
	}

	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->server, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
	while (code != 0)
	{
		if (code > 0)
		{
			switch (this->eNetEvent.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Server: Client connected" << std::endl;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				this->handleReceivedPacket();
				break;
			default:
				std::cout << "Warning: Server received unrecognized event type" << std::endl;
				break;
			}
		}
		else if (code < 0)
		{
			std::cout << "Error: Server service failed" << std::endl;
			break;
		}

		code = enet_host_service(this->server, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
	}

	// Vedem daca am pierdut conexiunea cu cineva.
	for (auto& connectedClient : this->connectedClients)
	{
		if (GlobalClock::get().getCurrentTime() - connectedClient.second.lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
		{
			connectedClient.second.workingConnection = false;
		}
		else
		{
			connectedClient.second.workingConnection = true;
		}
	}

	// Apoi trimitem ping-urile catre clienti.
	for (auto& connectedClient : this->connectedClients)
	{
		if (GlobalClock::get().getCurrentTime() - connectedClient.second.lastTimeSentPing < this->TIME_BETWEEN_PINGS)
			continue;

		nlohmann::json jsonData;
		jsonData["ping"] = true;

		connectedClient.second.sendMessageUnsafe(jsonData.dump());
	}

	// Mai vedem daca ne ramasese ceva de trimis la vreun client ce a esuat
	if (updateClients)
	{
		updateClients = false;

		for (auto& connectedClient : this->connectedClients)
		{
			nlohmann::json jsonData;

			for (auto& otherConnectedClient : this->connectedClients)
			{
				if (connectedClient.first == otherConnectedClient.first)
				{
					continue;
				}

				// TODO: deocamdata consideram ca vin toate atributele pt un player
				// TODO: verifica doar valorile noi

				// remotePlayers
				jsonData["remotePlayers"][otherConnectedClient.first]["clientName"] = otherConnectedClient.second.remotePlayerData.getClientName();
				jsonData["remotePlayers"][otherConnectedClient.first]["outfitColor"]["x"] = otherConnectedClient.second.remotePlayerData.getOutfitColor().x;
				jsonData["remotePlayers"][otherConnectedClient.first]["outfitColor"]["y"] = otherConnectedClient.second.remotePlayerData.getOutfitColor().y;
				jsonData["remotePlayers"][otherConnectedClient.first]["outfitColor"]["z"] = otherConnectedClient.second.remotePlayerData.getOutfitColor().z;
				jsonData["remotePlayers"][otherConnectedClient.first]["position"]["x"] = otherConnectedClient.second.remotePlayerData.getX();
				jsonData["remotePlayers"][otherConnectedClient.first]["position"]["y"] = otherConnectedClient.second.remotePlayerData.getY();
				jsonData["remotePlayers"][otherConnectedClient.first]["rotateAngle"] = otherConnectedClient.second.remotePlayerData.getRotateAngle();
				jsonData["remotePlayers"][otherConnectedClient.first]["statuses"] = otherConnectedClient.second.remotePlayerData.getStatuses();

				// bullets
				if (otherConnectedClient.second.bulletData.get())
				{
					bool isThrownGrenade = false;
					if (std::dynamic_pointer_cast<ThrownGrenade>(otherConnectedClient.second.bulletData))
					{
						isThrownGrenade = true;
					}

					jsonData["bullets"][otherConnectedClient.first]["isThrownGrenade"] = isThrownGrenade;
					jsonData["bullets"][otherConnectedClient.first]["x"] = otherConnectedClient.second.bulletData.get()->getX();
					jsonData["bullets"][otherConnectedClient.first]["y"] = otherConnectedClient.second.bulletData.get()->getY();
					jsonData["bullets"][otherConnectedClient.first]["rotateAngle"] = otherConnectedClient.second.bulletData.get()->getRotateAngle();
					jsonData["bullets"][otherConnectedClient.first]["speed"] = otherConnectedClient.second.bulletData.get()->getSpeed();
					jsonData["bullets"][otherConnectedClient.first]["textureName2D"] = otherConnectedClient.second.bulletData.get()->getTextureName2D();
					jsonData["bullets"][otherConnectedClient.first]["damage"] = otherConnectedClient.second.bulletData.get()->getDamage();
				}

				// sounds
				if (otherConnectedClient.second.soundData.get())
				{
					jsonData["sounds"][otherConnectedClient.first]["name"] = otherConnectedClient.second.soundData->name;
					jsonData["sounds"][otherConnectedClient.first]["paused"] = otherConnectedClient.second.soundData->paused;
				}

				// openedDoors
				if (otherConnectedClient.second.openedDoorData.get())
				{
					jsonData["openedDoors"][otherConnectedClient.first]["id"] = *otherConnectedClient.second.openedDoorData;
				}
			}

			// TODO: uncomment
			// std::cout << "SERVER send json: " << jsonData.dump() << std::endl;
			connectedClient.second.sendMessageUnsafe(jsonData.dump());
		}

		for (auto& connectedClient : this->connectedClients)
		{
			connectedClient.second.bulletData.reset(); // bulletData = nullptr
			connectedClient.second.soundData.reset(); // soundData = nullptr
			connectedClient.second.openedDoorData.reset(); // openedDoorData = nullptr
		}
	}

	// Eliminam din structura de date clientii pierduti. (Cu exceptia celui ce a initiat serverul, se poate pierde conexiunea daca dam drag and drop lent la fereastra aplicatiei.)
	for (auto connectedClient = this->connectedClients.begin(); connectedClient != this->connectedClients.end(); )
	{
		if (!connectedClient->second.workingConnection && connectedClient->second.lastTimeReceivedPing != 0.0f)
		{
			enet_peer_disconnect(connectedClient->second.peer, 0);
			connectedClient = this->connectedClients.erase(connectedClient);
		}
		else
		{
			++connectedClient;
		}
	}
}

void Server::stop()
{
	for (auto& connectedClient : this->connectedClients)
	{
		if (connectedClient.second.peer != nullptr)
			enet_peer_disconnect(connectedClient.second.peer, 0);
	}

	if (this->server != nullptr)
		enet_host_destroy(this->server);
	this->server = nullptr;

	this->address.host = ENET_HOST_ANY;
	this->address.port = 0;

	this->eNetEvent = ENetEvent();

	this->succesfullyCreated = false;
	this->lastTimeTriedCreation = 0.0f;

	this->connectedClients.clear();
}

void Server::sendMap(const std::string& clientKey)
{
	nlohmann::json jsonData;
	jsonData["map"] = map;

	connectedClients[clientKey].sendMessageUnsafe(jsonData.dump());
}

void Server::sendZombiesData(const std::unordered_map<std::string, std::shared_ptr<Enemy>>& remoteZombies)
{
	nlohmann::json jsonData;
	for (const auto& zombie : remoteZombies)
	{
		jsonData["zombies"][zombie.first]["position"]["x"] = zombie.second.get()->getX();
		jsonData["zombies"][zombie.first]["position"]["y"] = zombie.second.get()->getY();
		jsonData["zombies"][zombie.first]["rotateAngle"] = zombie.second.get()->getRotateAngle();
		jsonData["zombies"][zombie.first]["statuses"] = zombie.second.get()->getStatuses();
		jsonData["zombies"][zombie.first]["deleteEntity"] = zombie.second.get()->getDeleteEntity();
	}

	for (auto& connectedClient : this->connectedClients)
	{
		// std::cout << "SERVER send json: " << jsonData.dump() << std::endl;
		connectedClient.second.sendMessageUnsafe(jsonData.dump());
	}
}

void Server::sendNumFinishedWaves(int number)
{
	nlohmann::json jsonData;
	jsonData["waveNumber"] = number;

	for (auto& connectedClient : this->connectedClients)
	{
		// std::cout << "SERVER send json: " << jsonData.dump() << std::endl;
		connectedClient.second.sendMessageUnsafe(jsonData.dump());
	}
}
