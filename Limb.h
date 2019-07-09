#pragma once
#include "Entity.h"
class Limb :
	public Entity
{
public:

	float NewPosition[3] = { 0,0,0 };
	char LType;
	bool Falling;
	float rotationspeed;

	float StartRotation = 0;

	Limb(char Type, Entity* _Parent)
	{
		switch (Type)
		{
		case ARM_L:
			Visuals.Texture = 15;
			Name = "ARM";
			NewPosition[0] = 0.037f;
			break;
		case ARM_R:
			Visuals.Texture = 16;
			Name = "ARM";
			NewPosition[0] = -0.037f;
			break;
		case LEG_L:
			Visuals.Texture = 18;
			Name = "LEG";
			NewPosition[0] = 0.02f;
			NewPosition[1] = -0.045f;
			NewPosition[2] = 0.001f;
			StartRotation = 20;
			break;
		case LEG_R:
			Visuals.Texture = 19;
			Name = "LEG";
			NewPosition[0] = -0.02f;
			NewPosition[1] = -0.045f;
			NewPosition[2] = 0.001f;
			StartRotation = -20;
			break;
		case HEAD:
			Visuals.Texture = 1;
			NewPosition[1] = 0.035f;
			NewPosition[2] = 0.001f;
			break;
		case TORSO:
			Visuals.Texture = 14;
			break;
		case BLOOD:
			Visuals.Texture = 0;
			break;
		}

		if (Type != HEAD && Type != TORSO)
		{
			Visuals.VertexData[0] = -0.0125f;
			Visuals.VertexData[2] = 0.0125f;
			Visuals.VertexData[4] = 0.0125f;
			Visuals.VertexData[6] = -0.0125f;
		}

		if (Type == HEAD)
		{
			for (int i = 0; i < 8; i++)
			{
				Visuals.VertexData[i] *= 0.85;
			}
		}

		LType = Type;

		Parent = _Parent;
		SetStart(NewPosition);
		SetRotation(Z, StartRotation);
	}

	~Limb()
	{

	}

	void Start(){}

	void Update()
	{
		switch (LType)
		{
		case HEAD:
			switch (Parent->Facing)
			{
			case LEFT:
				Visuals.Texture = 5;
				break;
			case DOWN:
				Visuals.Texture = 1;
				break;
			case RIGHT:
				Visuals.Texture = 9;
				break;
			case UP:
				Visuals.Texture = 13;
				break;
			}
			break;
		case BLOOD:
			Visuals.Texture = 0;
			break;
		}
		if (Falling)
		{
			switch (LType)
			{
			case HEAD:
				switch (Parent->Facing)
				{
				case LEFT:
					Visuals.Texture = 8;
					break;
				case DOWN:
					Visuals.Texture = 4;
					break;
				case RIGHT:
					Visuals.Texture = 12;
					break;
				case UP:
					Visuals.Texture = 13;
					break;
				}
				break;
			case BLOOD:
				Visuals.Texture = 21;
				break;
			}
			Velocity[1] -= 0.001f;
			Rotation[3] = 1.0;
			Rotation[0] -= rotationspeed;
			switch (LType)
			{
			case ARM_L:
			case LEG_L:
				SetVelocity(X, 0.001f);
				
				break;
			case ARM_R:
			case LEG_R:
				SetVelocity(X, -0.001);
				Rotation[0] += 20.0;
				break;
			}
		}
	}

	void Destroy(){}

	void Reset()
	{
		Position[0] = StartPosition[0];
		Position[1] = StartPosition[1];
		Position[2] = StartPosition[2];
		SetVelocity(X, 0);
		SetVelocity(Y, 0);
		SetRotation(Z, StartRotation);
		Falling = false;
	}

	void Fling();
};

