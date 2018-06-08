#ifndef SERVER_H_
# define SERVER_H_

# include <map>
# include <memory>
# include "Map.h"
# include "Network.h"

class			    Server {
    public:
	Server(int port, int size_x = 10, int size_y = 10);
	~Server();

	bool		    Play();

    private:
	Network		    network;
	Map		    *map;
	std::vector<int>    graphics;

	bool 		ParseArguments(int id, std::string &argument);
	void 		UpdateGraphics();

	bool 		AddClient(int id, std::string &message);
	bool 		SendView(int id, std::string &message);
	bool 		ReceiveMessage(int id, std::string &message);
	bool 		MoveLeft(int id, std::string &message);
	bool		MoveRight(int id, std::string &message);
	bool 		MoveUp(int id, std::string &message);
	bool 		MoveDown(int id, std::string &message);
	bool 		Pick(int id, std::string &message);
	bool 		AddGraphics(int id, std::string &message);
	bool		SendBoard(int id, std::string &message);

	std::map<std::string, bool (Server::*)(int, std::string &)> protocol = {
	    { "200", &Server::AddClient },
	    { "201", &Server::SendView },
	    { "210", &Server::ReceiveMessage },
	    { "220", &Server::MoveLeft },
	    { "221", &Server::MoveRight },
	    { "222", &Server::MoveUp },
	    { "223", &Server::MoveDown },
	    { "230", &Server::Pick },
	    { "300", &Server::AddGraphics },
	    { "301", &Server::SendBoard }
	};
};

#endif /* SERVER_H_ */
