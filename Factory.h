#pragma once
#include "Character.h"
#include "Floor.h"
#include "Limb.h"
#include "CharacterFinder.h"

class Factory
{
public:

	Factory()
	{
	}

	~Factory()
	{
	}

	//Spawn a new Entity
	Entity* Spawn(char Object, char SubType, const float* Position, Entity* Parent)
	{
		switch (Object)
		{
		case PLAYER:
			return new Character(Position, SubType);
		case SCENERY:
			switch (SubType)
			{
			case ARROW:
				return new CharacterFinder(Parent);
			case FLOOR:
				return new Floor(Position);
			}
		case WEAPON:
			switch (SubType)
			{

			}
		case LIMB:
			return new Limb(SubType, Parent);
		}
	}
};


