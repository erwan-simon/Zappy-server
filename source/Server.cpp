#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Network.h"
#include "Map.h"
#include "Server.h"

Server::Server(int port, int size_x, int size_y) :
    network(Network(port)), map(new Map(size_x, size_y))
{
}

bool 		Server::AddClient(int id, std::string &)
{
    bool 	return_value;

    return_value = this->map->AddCharacter(id);
    if (return_value == true)
    {
	this->network.SendMessage(id, "100\n");
	return true;
    }
    this->network.SendMessage(id, "002 Client already exists.\n");
    return false;
}

bool 		Server::SendView(int, std::string &)
{
    return true;
}

bool 		Server::ReceiveMessage(int, std::string &)
{
    return true;
}

bool 		Server::MoveLeft(int, std::string &)
{
    return true;
}

bool 		Server::MoveRight(int, std::string &)
{
    return true;
}

bool 		Server::MoveUp(int, std::string &)
{
    return true;
}

bool 		Server::MoveDown(int, std::string &)
{
    return true;
}

bool 		Server::Pick(int, std::string &)
{
    return true;
}

bool 			Server::AddGraphics(int id, std::string &)
{
    std::string message;

    for (const auto &graphic : this->graphics)
    {
	if (graphic == id)
	{
	    std::cerr << "This graphic client already exists." << std::endl;
	    this->network.SendMessage(id, "002 Graphic client already exists.\n");
	    return false;
	}
    }
    this->graphics.push_back(id);
    message += "100 ";
    message += std::to_string(this->map->GetSizeX());
    message += ' ';
    message += std::to_string(this->map->GetSizeX());
    message += "\n";
    this->network.SendMessage(id, message);
    return true;
}

bool		Server::SendBoard(int id, std::string &)
{
    Box		**board = this->map->GetBoard();
    int		x = 0, y = 0;
    std::string table("101 [");

    for (y = 0; y != this->map->GetSizeY(); y += 1)
    {
	table += '[';
	for (x = 0; x != this->map->GetSizeX(); x += 1)
	{
	    table += '[';
	    table += "food:";
	    table += std::to_string(board[y][x].GetFood());
	    table += ']';
	}
	table += ']';
    }
    table += "]\n";
    this->network.SendMessage(id, table);
    return true;
}

bool		Server::SendCharacters(int id, std::string &)
{
    std::string table("104 [");

    for (const auto & character : this->map->GetCharacters())
    {
	table += "[id:";
	table += std::to_string(character.GetId());
	table += "][location_x:";
	table += std::to_string(character.GetLocation()[0]);
	table += "][location_y:";
	table += std::to_string(character.GetLocation()[1]);
	table += "][food:";
	table += std::to_string(character.GetFood());
	table += "]";
    }
    table += ']';
    this->network.SendMessage(id, table);
    return true;
}

bool	Server::ParseArguments(int id, std::string &argument)
{
    try {
	if (this->protocol.find(argument.substr(0, 3)) != this->protocol.end())
	    return ((this->*protocol[argument.substr(0, 3)])(id, argument));
	else
	{
	    std::cerr << argument.substr(0, 3) << ": protocol unknown." << std::endl;
	    return false;
	}
    }
    catch (std::out_of_range& e)
    {
	std::cerr << argument << "\nNo protocol number found..." << std::endl;
	return false;
    }
}

void 		Server::UpdateGraphics()
{
    bool 	here;

    std::vector<int>::iterator it = this->graphics.begin();
    while (it != this->graphics.end())
    {
	here = false;
	for (auto& client : this->network.GetClients())
	{
	    if (*it == client)
		here = true;
	}
	if (here == false)
	    it = graphics.erase(it);
	else
	    ++it;
    }
}

bool Server::Play()
{
    while (true)
    {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	this->network.ReadFromClients();
	this->map->UpdateCharacters(this->network.GetClients());
	this->map->FoodRain();
	this->UpdateGraphics();
	if (this->network.GetBuffer().size() != 0)
	{
	    for (auto& message : this->network.GetBuffer())
	    {
		this->ParseArguments(message.first, message.second);
	    }
	    this->network.ClearBuffer();
	}
    }
}

Server::~Server()
{
    delete this->map;
}
