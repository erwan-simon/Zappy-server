#include "Network.h"
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

Network::Network(int port) :
	port(port)
{
	// create a socket
	// socket(int domain, int type, int protocol)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// non blocking socket
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	if (sockfd < 0)
		std::cerr << "ERROR opening socket" << std::endl;
	/* setup the host_addr structure for use in bind call */
	// server byte order
	serv_addr.sin_family = AF_INET;

	// automatically be filled with current host's IP address
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// convert short integer value for port must be converted into network byte order
	serv_addr.sin_port = htons(port);

	// bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
	// bind() passes file descriptor, the address structure,
	// and the length of the address structure
	// This bind() call will bind  the socket to the current IP address on port, portno
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		 sizeof(serv_addr)) < 0)
	{
		std::perror("bind");
		throw 1;
	}
	/* Create the socket and set it up to accept connections. */
	if (listen (sockfd, 1) < 0)
	{
		perror ("listen");
		exit (EXIT_FAILURE);
	}
	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
}

bool Network::ReceiveMessage(int id, std::string & message)
{

	char buffer;
	int nbytes;
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

void Network::ReadFromClients()
{
	std::string *message = new std::string;
	fd_set read_fd_set;
	FD_SET (sockfd, &active_fd_set);
	/* Block until input arrives on one or more active sockets. */
	read_fd_set = active_fd_set;
	if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
	{
		std::perror("select");
		throw 3;
	}
	/* Service all the sockets with input pending. */
	for (int i = 0; i < FD_SETSIZE; ++i)
		if (FD_ISSET (i, &read_fd_set))
		{
			if (i == sockfd)
			{
				int id = this->AddClient();
				FD_SET (id, &active_fd_set);
			}
			else
			{
				/* Data arriving on an already-connected socket. */
				if (this->ReceiveMessage(i, *message) == true)
					std::cout << "Receive " << *message << " from " << i << std::endl;
			}
		}
}

int Network::AddClient()
{
	struct sockaddr_in client_addr;
	socklen_t clilen;
	listen(sockfd, 5);
	int newsockfd = accept(sockfd,
			   (struct sockaddr *) &client_addr, &clilen);
	if (newsockfd < 0)
		return -1;
	clients.push_back(newsockfd);
	std::cout << "Added " << newsockfd << std::endl;
	return newsockfd;
}

void Network::RemoveClient(int id)
{
	std::cout << "Remove " << id << std::endl;
	clients.erase(std::find(clients.begin(), clients.end(), id));
	close(id);
	FD_CLR(id, &active_fd_set);
}

Network::~Network()
{
	close(sockfd);
}
