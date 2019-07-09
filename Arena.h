#pragma once
#include "Level.h"
class Arena :
	public Level
{
public:

	float PlayerStartLocation[3] = { 0.4f, 0, -1.0f };
	float NewLocation[3] = { -0.5f, 0.0f, -1.0f };

	float Tile[3] = { -0.5f, 0, -1.1f };
	Entity* cf;

	int Timer = 0;

	void Start()
	{
		SpawnCharacter(1, PlayerStartLocation);
		SpawnCharacter(0, NewLocation);
	}

	void Update()
	{
		Timer++;

		if (Timer == 241)
		{
			SpawnCharacter(0, NewLocation);
			Timer = 0;
		}
	}
};

