/* The port number is passed as an argument */

#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include "Network.h"
#include "Character.h"
#include "Map.h"


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "No port given." << std::endl;
		return 1;
	}
	if (std::atoi(argv[1]) == 0)
	{
		std::cerr << "Wrong port given." << std::endl;
		return 1;
	}
	try {
		Network network(std::atoi(argv[1]));
		Map map(10, 10);
		std::string *message = new std::string;
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			network.ReadFromClients();
			map.UpdateCharacters(network.GetClients());
			if (network.GetClients().size() != 0)
			{
				std::cout << "enter something" << std::endl;
				std::getline(std::cin, *message);
				network.SendMessage(network.GetClients().back(), *message);
			}
		}
		return 0;
	} catch (int n)
	{
		std::cerr << "Something happened, exiting..." << std::endl;
		return 1;
	}
}
