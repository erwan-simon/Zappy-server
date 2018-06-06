#include "Character.h"

Character::Character(int id) :
	id(id)
{
}

int Character::GetId() const
{
	return (this->id);
}

Character::~Character()
{
}
