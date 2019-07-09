#pragma once
#include "Limb.h"
#include "Level.h"

void Limb::Fling()
{
	Falling = true;
	if (LType == BLOOD)
	{
		SetVelocity(X, (rand() % 100) - 50);
	}

	SetVelocity(Y, 0.01f);
	rotationspeed = (rand() % 50) - 25;
}