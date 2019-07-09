#pragma once
#include "Entity.h"
class Floor :
	public Entity
{
public:

	Floor(const float* _Position)
	{
		SetStart(_Position);
		Visuals.Texture = 17;
		Name = "FLOOR";
	}

	void Start()	{}
	void Update()	{}
	void Destroy(){}
};

