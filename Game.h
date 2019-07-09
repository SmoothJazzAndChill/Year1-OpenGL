#pragma once
#include "Level.h"
#include "Arena.h"
#include "Character.h"

class Game
{
public:
	Renderer* R;
	Factory* F;
	Level* _Arena;

	Game()
	{
		F = new Factory();
		R = new Renderer();
		_Arena = new Arena();
		_Arena->SetRenderer(R);
		_Arena->SetFactory(F);
		_Arena->ConstructLevel();
	}

	~Game()
	{
		delete R;
		R = NULL;
		delete F;
		F = NULL;
		delete _Arena;
		_Arena = NULL;
	}

	void Update()
	{
		_Arena->ContentsUpdate();
	}
};