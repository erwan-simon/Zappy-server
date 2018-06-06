#include "Network.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

Network::Network(int port) :
	port(port)
{
	// create a socket
	// socket(int domain, int type, int protocol)
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// non blocking socket
	fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
	if (this->sockfd < 0)
		std::cerr << "ERROR opening socket" << std::endl;
	/* setup the host_addr structure for use in bind call */
	// server byte order
	this->serv_addr.sin_family = AF_INET;

	// automatically be filled with current host's IP address
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;

	// convert short integer value for port must be converted into network byte order
	this->serv_addr.sin_port = htons(this->port);

	// bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
	// bind() passes file descriptor, the address structure,
	// and the length of the address structure
	// This bind() call will bind  the socket to the current IP address on port, portno
	if (bind(this->sockfd, (struct sockaddr *) &this->serv_addr,
		 sizeof(this->serv_addr)) < 0)
	{
		std::perror("bind");
		throw 1;
	}
	/* Create the socket and set it up to accept connections. */
	if (listen (this->sockfd, 10) < 0)
	{
		perror ("listen");
		exit (EXIT_FAILURE);
	}
	/* Initialize the set of active sockets. */
	FD_ZERO (&this->active_fd_set);
	FD_SET(this->sockfd, &this->active_fd_set);
}

bool 		Network::SendMessage(int id, std::string const & message)
{
	int 	return_value = 0;

	if (message.back() == '\n')
		return_value = dprintf(id, "%s", message.c_str());
	else
		return_value = dprintf(id, "%s\n", message.c_str());
	if (return_value < 0)
	{
		std::cerr << "Couldn't send message to player " << id << "." << std::endl;
		return false;
	}
	return true;
}

bool 		Network::ReceiveMessage(int id, std::string & message)
{

	char 	buffer;
	int 	nbytes;

	while (true)
	{
		nbytes = read(id, &buffer, 1);
		if (nbytes < 0)
		{
			/* Read error. */
			std::perror("read");
			throw 2;
		}
		else if (nbytes == 0)
		{
			/* End-of-file. */
			this->RemoveClient(id);
			return false;
		}
		else
		{
			if (buffer == '\n')
				return true;
			/* Data read. */
			message += buffer;
		}
	}
}

void 								Network::ReadFromClients()
{
	std::unique_ptr<std::string> 	message(new std::string);
	fd_set 							read_fd_set;
	int 							i;
	struct timeval  				tv = {0, 50};

	read_fd_set = this->active_fd_set;
	/* Block until input arrives on one or more active sockets. */
	if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, &tv) < 0)
	{
		std::perror("select");
		throw 3;
	}
	/* Service all the sockets with input pending. */
	for (i = 0; i < FD_SETSIZE; ++i)
	{
		if (FD_ISSET (i, &read_fd_set))
		{
			if (i == this->sockfd)
				this->AddClient();
			else
			{
				/* Data arriving on an already-connected socket. */
				if (this->ReceiveMessage(i, *message) == true)
					std::cout << "Receive '" << *message << "' from player " << i << std::endl;
			}
		}
	}
}

int 					Network::AddClient()
{
	struct sockaddr_in	client_addr;
	int 				new_sockfd;

	socklen_t clilen;
	listen(this->sockfd, 5);
	new_sockfd = accept(this->sockfd,
			(struct sockaddr *) &client_addr, &clilen);
	if (new_sockfd < 0)
		return -1;
	this->clients.push_back(new_sockfd);
	std::cout << "Player " << new_sockfd << " joined the game."<< std::endl;
	FD_SET(new_sockfd, &this->active_fd_set);
	return new_sockfd;
}

void Network::RemoveClient(int id)
{
	std::cout << "Player " << id << " left the game." << std::endl;
	this->clients.erase(std::find(this->clients.begin(), this->clients.end(), id));
	close(id);
	FD_CLR(id, &this->active_fd_set);
}

Network::~Network()
{
	close(this->sockfd);
}

std::vector<int> const & Network::GetClients()
{
	return this->clients;
}
