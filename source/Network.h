#ifndef NETWORK_H_
# define NETWORK_H_

#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class 								Network {
	public:
		Network(int port);
		~Network();
		void 										ReadFromClients();
		bool 										SendMessage(int id, std::string const & message);
		void 										RemoveClient(int id);
		std::vector<int> const &					GetClients();
		std::vector<std::pair<int, std::string>> &	GetBuffer();
		void 										ClearBuffer();

	private:
		int 										port;
		struct sockaddr_in  						serv_addr;
		int 										sockfd;
		std::vector<int> 							clients;
		fd_set 										active_fd_set;
		std::vector<std::pair<int, std::string>>	buffer;

		int 										AddClient();
		bool 										ReceiveMessage(int id);
};

#endif /* NETWORK_H_ */
