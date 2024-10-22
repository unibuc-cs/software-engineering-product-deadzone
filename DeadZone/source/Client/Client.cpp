#include "Client.h"

#include <enet/enet.h>

Client::Client()
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

