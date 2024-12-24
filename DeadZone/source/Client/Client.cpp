#include "Client.h"

#include <nlohmann/json.hpp>

#include <iostream>

#include "../GlobalClock/GlobalClock.h"
#include "../Game/Game.h"
#include "../Entity/Player/Player.h"

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

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_UNSEQUENCED);

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

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_UNSEQUENCED);

	// 0 daca a avut succes
	if (enet_peer_send(this->serverPeer, 0, packet) == 0)
	{
		timeWhenMessageSent = GlobalClock::get().getCurrentTime();

		// TODO: uncomment
		// std::cout << "Client sent message: " << messageToSend << std::endl;
	}
	else
		std::cout << "Error: Client failed to send message" << std::endl;
}

bool Client::shouldSendRemotePlayerData()
{
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
	// TODO

	return shouldSend;
}

void Client::handleReceivedPacket()
{
	// TODO: uncomment
	// std::cout << "In handleReceivedPacket in Client named " << this->clientName << std::endl;

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

			// playerData["clientName"]
			// playerData["outfitColor"]
			Game::get().updateRemotePlayerPosition(clientKey, playerData["position"]["x"].get<double>(), playerData["position"]["y"].get<double>());
			// playerData["statuses"]
		}
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

	// Trimitem ce informatii vitale stim deja catre server.
	if (shouldSendRemotePlayerData())
	{
		nlohmann::json jsonData;

		jsonData["clientName"] = this->clientName;
		// TODO: outfitColor
		jsonData["position"]["x"] = Player::get().getX();
		jsonData["position"]["y"] = Player::get().getY();
		// TODO: statuses

		sendMessageUnsafe(jsonData.dump(), this->lastTimeSentPing);
	}

	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->client, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
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
	//if (this->client != nullptr)
	//	enet_host_flush(this->client);
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

