#pragma once
#include "Entity.h"
class CharacterFinder :
	public Entity
{
public:

	int TextureID = 20;
	float NewPosition[3] = { 0,0,-1.001f };
	CharacterFinder(Entity* _Parent)
	{
		Name = "ARROW";
		Visuals.Texture = TextureID;
		Parent = _Parent;

		SetStart(NewPosition);
	}

	~CharacterFinder()
	{
	}

	void Start()
	{
		for (int i = 0; i < 8; i++)
		{
			Visuals.VertexData[i] *= 0.8;
		}
	}

	const float* pPosition;
	void Update()
	{
		Visuals.Texture = 0;
		pPosition = Parent->GetPosition();

		if (pPosition[0] >= 0.7f)
		{
			Visuals.Texture = TextureID;
			Position[0] = 0.7f;
		}
		else if (pPosition[0] <= -0.7f)
		{
			Visuals.Texture = TextureID;
			Position[0] = -0.7f;
		}
		else
		{
			Position[0] = pPosition[0];
		}

		if (pPosition[1] >= 0.35f)
		{
			Visuals.Texture = TextureID;
			Position[1] = 0.35f;
		}
		else if (pPosition[1] <= -0.35f)
		{
			Visuals.Texture = TextureID;
			Position[1] = -0.35f;
		}
		else
		{
			Position[1] = pPosition[1];
		}

		float angle = (float)std::atan2(Position[1] - pPosition[1], Position[0] - pPosition[0]) * 180/3.14 + 90;
		SetRotation(Z, angle);
	}

	void Destroy()
	{

	}
};

