#pragma once

#include <enet/enet.h>
#include <string>
#include <map>
#include <memory>

#include "../Entity/RemotePlayer/RemotePlayer.h"
#include "../Entity/Player/Player.h"
#include "../Entity/Bullet/Bullet.h"
#include "../Entity/Enemy/Enemy.h"

struct ReplicatedSound
{
	std::string name;
	bool paused;

	// Constructor
	ReplicatedSound(const std::string& name, bool paused);
};

class Server
{
private:
	struct ClientData
	{
		ENetPeer* peer;
		RemotePlayer remotePlayerData;
		std::shared_ptr<Bullet> bulletData;
		std::shared_ptr<ReplicatedSound> soundData;
		std::shared_ptr<unsigned int> openedDoorData;
		float lastTimeSentPing;
		float lastTimeReceivedPing;
		bool workingConnection;

		ClientData()
			: peer(nullptr)
			, lastTimeSentPing(0.0f)
			, lastTimeReceivedPing(0.0f)
			, workingConnection(false)
			, remotePlayerData(10.5, 10.5, 1.0, 1.0, 0.0, 5.0, 0.4, 0.4, Player::ANIMATIONS_NAME_2D, Player::STATUSES, 7.5)
			, bulletData(nullptr)
			, soundData(nullptr)
		{

		}

		void sendMessage(const std::string& messageToSend, bool& failedToSendMessage);
		void sendMessageUnsafe(const std::string& messageToSend);
	};

private:
	Server();
	~Server();
	Server(const Server& other) = delete;
	Server& operator= (const Server& other) = delete;
	Server(const Server&& other) = delete;
	Server& operator= (const Server&& other) = delete;

	const int MAX_NUM_CLIENTS;
	const int NUM_CHANNELS;
	const int TIME_WAITING_FOR_EVENTS_MS;
	ENetHost* server;
	ENetAddress address;

	const int MINIMUM_PORT;
	const int MAXIMUM_PORT;

	ENetEvent eNetEvent;

	bool succesfullyCreated;
	float lastTimeTriedCreation;
	const float RETRY_CREATION_DELTA_TIME;

	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;

	bool updateClients;
	std::map<std::string, ClientData> connectedClients;
	std::vector<std::vector<std::string>> map;

	// Atentie aici la unicitatea cheii
	inline std::string getClientKey(const ENetAddress& address) const { return std::to_string(address.host) + ":" + std::to_string(address.port); }

	void handleReceivedPacket();

	void generateMap();

public:
	static Server& get();

	void start();
	void start(const std::string& serverPort);
	void update();
	void stop();

	enet_uint16 getPort() const { return this->address.port; }

	void sendMap(const std::string& clientKey);
	void sendZombiesData(const std::unordered_map<std::string, std::shared_ptr<Enemy>>& remoteZombies);
	void sendNumFinishedWaves(int number);
};

