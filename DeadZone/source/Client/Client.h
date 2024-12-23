#pragma once

#include <string>
#include <enet/enet.h>

class Client
{
private:
	Client();
	~Client();
	Client(const Client& other) = delete;
	Client& operator= (const Client& other) = delete;
	Client(const Client&& other) = delete;
	Client& operator= (const Client&& other) = delete;

	const int MAX_NUM_SERVERS; // Numarul maxim de server-e la care poate fi clientul conectat
	const int NUM_CHANNELS;
	const int TIME_WAITING_FOR_EVENTS_MS;

	ENetPeer* serverPeer;
	ENetHost* client;
	ENetAddress serverAddress;
	ENetEvent eNetEvent;

	bool succesfullyConnected;
	float lastTimeTriedConnection;
	const float RETRY_CONNECTION_DELTA_TIME;


	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;
	float lastTimeReceivedPing;
	float lastTimeSentPing;

	std::string clientName;

	bool workingServerConnection;

	bool hasToSendName;

	void handleReceivedPacket();

public:
	static Client& get();

	void start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName);
	void update();
	void stop();

	void sendMessage(const std::string& messageToSend, bool& failedToSendMessage, float& timeWhenMessageSent);
	void sendMessageUnsafe(const std::string& messageToSend, float& timeWhenMessageSent);

	inline bool getWorkingServerConnection() const { return this->workingServerConnection; }
};

