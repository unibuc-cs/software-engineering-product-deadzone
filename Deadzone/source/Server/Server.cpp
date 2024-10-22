#include "Server.h"

#include <enet/enet.h>

Server::Server()
{

}

Server::~Server()
{

}

Server& Server::get()
{
	static Server instance;

	return instance;
}

