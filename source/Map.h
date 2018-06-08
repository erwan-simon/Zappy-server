#ifndef MAP_H_
# define MAP_H_

# include <vector>
# include <memory>
# include "Box.h"
# include "Character.h"

class 					Map {
    public:
	Map(int size_x, int size_y);
	~Map();
	void				FoodRain();
	bool				AddCharacter(int id);
	bool 				RemoveCharacter(int id);
	void				UpdateCharacters(std::vector<int> const& clients);
	Box 	 			**GetBoard() const;
	std::vector<Character> const&	GetCharacters() const;
	int 				GetSizeX() const;
	int 				GetSizeY() const;
	Map&				operator=(Map const & other);

    private:
	int 				size_x;
	int 				size_y;
	Box 				**board; // board[y][x]
	std::vector<Character>		characters;
};

#endif /* MAP_H_ */
