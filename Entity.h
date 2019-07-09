#pragma once
#include <vector>
#include <windows.h>
#include <math.h>
#include <iostream>
#include "EventHandler.h"

class Level;

enum OBJECTS { PLAYER, SCENERY, WEAPON, LIMB };
enum WORLD { FLOOR, ARROW };
enum WEAPONS { SWORD, CHAINSAW };
enum LIMBS { ARM_L, ARM_R, LEG_L, LEG_R, HEAD, TORSO, BLOOD };

enum LOOKDIRECTIONS { DOWN, LEFT, UP, RIGHT };

struct Visual
{
public:
	float VertexData[8] = {
		-0.025f,	-0.025f,
		0.025f,	-0.025f,
		0.025f,	0.025f,
		-0.025f,	0.025f };

	unsigned int Texture;

	//Swap between textures to animate it.
	void AnimateTexture(int StartID, int EndID, int Rate)
	{
		fCounter++;
		if (fCounter == Rate)
		{
			Texture++;
			if (Texture > EndID) Texture = StartID;
			fCounter = 0;
		}
	}

private:
	//Frame counter
	int fCounter;
};

class Entity
{
public:
	Entity()
	{
		
	}

	~Entity()
	{
		delete Handler;
		Handler = NULL;
	}

	//A name for the Entity
	std::string Name;

	//Customisable Start function
	virtual void Start()=0;
	//Customisable Update function
	virtual void Update()=0;
	//What happens when the object is destroyed (not deleted)
	virtual void Destroy()=0;

	//Runs when the Entity is created
	void Initialise(Level* l)
	{
		SetLevel(l);
		Position[0] = StartPosition[0];
		Position[1] = StartPosition[1];
		Position[2] = StartPosition[2];
		Initialised = true;

		Start();
	}

	//Update run every frame
	void EntityUpdate()
	{
		if (PlayerControlled)
		{
			Handler->ReadInput();

			for (char c : Handler->Keys)
			{
				if (Handler->ContainsKey(0, c))
				{
					KeyDown(c);
					continue;
				}

				if (Handler->ContainsKey(1, c))
				{
					KeyUp(c);
					Handler->RemovedKeys.remove(c);
				}
			}
		}

		Position[0] += Velocity[0];
		Position[1] += Velocity[1];
		Position[2] += Velocity[2];

		Update();
	}

	virtual void KeyDown(char key) {};
	virtual void KeyUp(char key) {};
	
	virtual void MouseDown(char key) {};
	virtual void MouseUp(char key) {};

	//For readability as it's a boolean
	enum CONTROLLER { COMPUTER, PLAYER };
	//Returns the position of the Entity
	const float* GetPosition()	{ return Position; };
	//Returns the rotation of the Entity
	const float* GetRotation()	{ return Rotation; };
	//Returns the scale of the Entity (scale nyi)
	const float* GetScale()		{ return Scale; };
	//Returns the velocity of the Entity
	const float* GetVelocity()	{ return Velocity; };
	//Returns the controller of the Entity. True for Player, False for AI
	bool		 GetController(){ return PlayerControlled; };

	//For readability and ease of use
	enum DIRECTIONS {X = 0, Y, Z};
	//Sets the position of the Entity
	void SetPosition(char Dir, float Value) { Position[Dir] = Value; };
	//Sets the velocity of the Entity
	void SetVelocity(char Dir, float Value) { Velocity[Dir] = Value; };
	//Sets the rotation of the Entity
	void SetRotation(char Dir, float Angle) { Rotation[0] = Angle; Rotation[Dir + 1] = 1.0f; };

	//Set the start position of the Entity
	void SetStart(const float pos[3])		{ StartPosition = pos; };

	//Provides the entity with informatio about the level
	void SetLevel(Level* l) { CurrentLevel = l; };

	//Set who controls the entity
	void SetController(bool set)	{ PlayerControlled = set; };
	Visual Visuals;
	Entity* Parent;

	bool Initialised = false;
	char Facing;

protected:
	const float* StartPosition;
	float Position[3] = { 0,0,0 };
	float Rotation[4] = { 0,0,0,0 };
	float Scale[3] = { 0,0,0 };
	float Velocity[3] = { 0,0,0 };
	float MovementSpeed;
	bool PlayerControlled = false;
	EventHandler* Handler = new EventHandler();
	Level* CurrentLevel;
};