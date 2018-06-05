#ifndef NETWORK_H_
# define NETWORK_H_

#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Network {
	public:
		Network(int port);
		~Network();
		int AddClient();
		void RemoveClient(int id);
		void ReadFromClients();

	private:
		int port;
		struct sockaddr_in serv_addr;
		int sockfd;
		std::vector<int> clients;
		fd_set active_fd_set;

		bool ReceiveMessage(int id, std::string & message);
};

#endif /* NETWORK_H_ */
