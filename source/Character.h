#ifndef CHARACTER_H_
# define CHARACTER_H_

class 			Character {
	public:
		Character(int id);
		~Character();
		int 	GetId() const;

	private:
		int 	id;
		int 	location[2] = {0, 0};
		int 	food = 5;
};

#endif /* CHARACTER_H_ */
