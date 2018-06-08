#include <algorithm>
#include <vector>
#include "Map.h"

Map::Map(int size_x, int size_y) :
    size_x(size_x), size_y(size_y)
{
    int 	x, y;

    this->board = new Box*[size_y];
    for (y = 0; y != this->size_y; y += 1)
    {
	this->board[y] = new Box[size_x];
	for (x = 0; x != this->size_x; x += 1)
	    this->board[y][x] = Box();
    }
}

Map::~Map()
{
    int 	y;

    for (y = 0; y != this->size_y; y += 1)
	delete this->board[y];
    delete this->board;
}

bool 	Map::AddCharacter(int id)
{
    for (auto &character : this->characters)
    {
	if (character.GetId() == id)
	    return false;
    }
    this->characters.push_back(Character(id, *this));
    return true;
}

bool 	Map::RemoveCharacter(int id)
{
    std::vector<Character>::iterator it = this->characters.begin();
    while (it != this->characters.end())
    {
	if (it->GetId() == id)
	{
	    this->characters.erase(it);
	    return true;
	}
	++it;
    }
    return false;
}

void 		Map::UpdateCharacters(std::vector<int> const& clients)
{
    bool	here;

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

Box 	**Map::GetBoard() const
{
    return(board);
}

std::vector<Character> const&	Map::GetCharacters() const
{
    return (this->characters);
}

int 	Map::GetSizeX() const
{
    return (this->size_x);
}

int 	Map::GetSizeY() const
{
    return (this->size_y);
}

Map&	Map::operator=(Map const & other)
{
    int x = 0, y = 0;

    if (&other == this)
	return *this;
    // deleting previous table
    for (y = 0; y != this->size_y; y += 1)
	delete this->board[y];
    delete this->board;
    this->size_x = other.size_x;
    this->size_y = other.size_y;
    // filling new table
    this->board = new Box*[size_y];
    for (y = 0; y != this->size_y; y += 1)
    {
	this->board[y] = new Box[size_x];
	for (x = 0; x != this->size_x; x += 1)
	    this->board[y][x] = other.GetBoard()[y][x];
    }
    this->characters = other.characters;
    return *this;
}
