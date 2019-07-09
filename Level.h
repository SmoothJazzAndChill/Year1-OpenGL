#pragma once
#include "Factory.h"
#include "Renderer.h"

#ifndef LEVEL_H
#define LEVEL_H


class Level
{
public:

	Level()
	{
		
	}

	~Level()
	{
		_Renderer = NULL;
		_Factory = NULL;
		for (Entity* e : LevelContents)
		{
			delete e;
			e = NULL;
		}
		LevelContents.clear();
		
	}

	void SetRenderer(Renderer* R)	{ _Renderer = R; };
	void SetFactory(Factory* F)		{ _Factory = F; };

	int ViolenceLevel = 30;

	virtual void Start()=0;
	virtual void Update()=0;

	void ContentsUpdate()
	{
		Update();

		for (Entity* e : LevelContents)
		{
			e->EntityUpdate();
			_Renderer->Queue(e);
		}
		_Renderer->Draw();
	}

	std::vector<Entity*>* GetLevelContents() { return &LevelContents; }

	void AddEntity(Entity* target)
	{
		target->Initialise(this);
		LevelContents.push_back(target);
	}

	void ConstructLevel()
	{
		Start();
		for (Entity* e : LevelContents)
		{
			e->Initialise(this);
		}
	}

	void SpawnCharacter(bool PlayerControlled, const float* Position)
	{
		CharacterHolder = _Factory->Spawn(PLAYER, PlayerControlled, Position, NULL);
		AddEntity(_Factory->Spawn(LIMB, HEAD, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(LIMB, ARM_L, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(LIMB, ARM_R, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(LIMB, LEG_L, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(LIMB, LEG_R, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(LIMB, TORSO, LimbLocation, CharacterHolder));
		AddEntity(_Factory->Spawn(SCENERY, ARROW, LimbLocation, CharacterHolder));
		/*
		for (int i = 0; i < ViolenceLevel; i++)
			AddEntity(_Factory->Spawn(LIMB, BLOOD, LimbLocation, CharacterHolder));
			*/
		AddEntity(CharacterHolder);
		CharacterHolder = NULL;
	}

	Renderer* _Renderer;
	Factory* _Factory;

protected:
	std::vector<Entity*> LevelContents;

	Entity* CharacterHolder;
	float LimbLocation[3] = {0,0,0};
	
};

#endif