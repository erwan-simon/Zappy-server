/* The port number is passed as an argument */

#include <iostream>
#include <cstdlib>
#include "Server.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "No port given." << std::endl;
		return 1;
	}
	try {
		Server server(std::atoi(argv[1]));
		server.Play();
	}
	catch (int n)
	{
		std::cerr << "Something happened, exiting..." << std::endl;
		return 1;
	}
	return 0;
}
