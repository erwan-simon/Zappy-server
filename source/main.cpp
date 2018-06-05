/* The port number is passed as an argument */

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "Network.h"
#include "Character.h"



int main(int argc, char *argv[])
{
	std::vector<Character *> characters;

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
		Network *network = new Network(std::atoi(argv[1]));
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			network->ReadFromClients();	
			/*
			int id = network->AddClient();
			if (id > 0)
			{
				characters.push_back(new Character(id));
				network->ReadFromClients();
				std::cout << "client added" << std::endl;
			}
			*/
		}
		return 0;
	} catch (int n)
	{
		std::cerr << "Something happened, exiting..." << std::endl;
		return 1;
	}
}
