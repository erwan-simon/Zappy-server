#ifndef MAP_H_
# define MAP_H_

# include <vector>
# include <memory>
# include "Box.h"
# include "Character.h"

class 							Map {
	public:
		Map(int size_x, int size_y);
		~Map();

		bool					AddCharacter(int id);
		bool 					RemoveCharacter(int id);
		void					UpdateCharacters(std::vector<int> const& clients);

	private:
		int 					size_x;
		int 					size_y;
		std::shared_ptr<Box> 	**board;
		std::vector<Character>	characters;
};

#endif /* MAP_H_ */
