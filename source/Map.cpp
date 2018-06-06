#include <algorithm>
#include <vector>
#include "Map.h"

Map::Map(int size_x, int size_y) :
	size_x(size_x), size_y(size_y)
{
	int 	x, y;

	this->board = new std::shared_ptr<Box>*[size_y];
	for (y = 0; y != this->size_y; y += 1)
	{
		this->board[y] = new std::shared_ptr<Box>[size_x];
		for (x = 0; x != this->size_x; x += 1)
			this->board[y][x] = std::shared_ptr<Box>(new Box);
	}
}

Map::~Map()
{
}

bool 	Map::AddCharacter(int id)
{
	for (auto &character : this->characters)
	{
		if (character.GetId() == id)
			return false;
	}
	this->characters.push_back(Character(id));
	return true;
}

bool 	Map::RemoveCharacter(int id)
{
	for(std::vector<Character>::iterator it = this->characters.begin(); it != this->characters.end(); ++it)
	{
		if (it->GetId() == id)
		{
			this->characters.erase(it);
			return true;
		}
	}
	return false;
}

void 		Map::UpdateCharacters(std::vector<int> const& clients)
{
	bool 	here;

	for (const auto& client : clients)
	{
		here = false;
		for (const auto& character : this->characters)
		{
			if (character.GetId() == client)
				here = true;
		}
		if (here == false)
			this->AddCharacter(client);
	}
	std::vector<Character>::iterator it = this->characters.begin();
	while (it != this->characters.end())
	{
		here = false;
		for (const auto& client : clients)
		{
			if (it->GetId() == client)
				here = true;
		}
		if (here == false)
			it = this->characters.erase(it);
		else
			++it;
	}
}
