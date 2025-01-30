#pragma once

#include <enet/enet.h>

#include <string>
#include <memory>

#include "../Entity/Bullet/Bullet.h"
#include "../Entity/RemotePlayer/RemotePlayer.h"

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
	RemotePlayer lastRemotePlayerData;

	bool shouldSendRemotePlayerData();
	void handleReceivedPacket();

	void sendMessage(const std::string& messageToSend, bool& failedToSendMessage, float& timeWhenMessageSent);
	void sendMessageUnsafe(const std::string& messageToSend, float& timeWhenMessageSent);

public:
	static Client& get();

	void start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName);
	void update();
	void stop();

	void sendBullet(const std::shared_ptr<Bullet>& const entity);
	void sendSound(const std::string& name, bool paused);
	void sendOpenedDoor(int id);
	void sendCloseRangeDamage(const double damage, const double shortRangeAttackRadius);
	void sendDisconnect();
	void sendConfirmedKill(const std::string& clientKey);

	// Getters
	inline bool getWorkingServerConnection() const { return this->workingServerConnection; }
};

