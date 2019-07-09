#include "Character.h"
#include "Level.h"
#include "Limb.h"

void Character::Attack()
{
	float Distance = 0;
	float FaceVector[3] = { 0,0,0 };
	float Direction[3] = { 0,0,0 };
	float Dot = 0;

	float angle;
	const float* ePosition;

	if (!AttackCoolDown)
	{
		if (Facing == UP) FaceVector[1] = 1;
		if (Facing == DOWN) FaceVector[1] = -1;
		if (Facing == LEFT) FaceVector[0] = -1;
		if (Facing == RIGHT) FaceVector[0] = 1;

		for (Entity* e : *(CurrentLevel->GetLevelContents()))
		{
			if (e->Name == "PLAYER" && e != this)
			{
				Character* a = (Character*)e;
				ePosition = a->GetPosition();
				Distance = GetDistance(ePosition);
				/*
				angle = (float)std::atan2(Position[1] - ePosition[1], Position[0] - ePosition[0]);

				switch (Facing)
				{
				case UP:
					angle += 90;
					break;
				case LEFT:
					angle += 180;
					break;
				case DOWN:
					angle -= 90;
					break;
				}

				Direction[0] = e->GetPosition()[0] * (1 / Distance);
				Direction[1] = e->GetPosition()[1] * (1 / Distance);
				Direction[2] = e->GetPosition()[2] * (1 / Distance);

				Dot = ((FaceVector[0] * Direction[0]) + (FaceVector[1] * Direction[1]) + (FaceVector[2] * Direction[2]));

				if (Dot >= cos(3.14 / 2) && Distance <= Range)
				{
					a->TakeDamage(Damage);
				}
				*/

				if (Distance <= Range && a->Alive)
				{
					a->TakeDamage(Damage);
				}
			}
		}
		AttackCoolDown = true;
	}
}

void Character::GetTarget()
{
		for (Entity* e : *(CurrentLevel->GetLevelContents()))
		{
			if (e->Name == "PLAYER" && e != this)
			{
				Character* c = (Character*)e;
				if (c->Alive)
				{
					if (target == NULL) target = c;
					if (GetDistance(e->GetPosition()) < GetDistance(target->GetPosition()))
					{
						target = c;
					}
				}
			}
		}
}

void Character::GetLimbs()
{
	for (Entity* e : *(CurrentLevel->GetLevelContents()))
	{
		if (e->Parent == this) Limbs.push_back(e);
	}
}

void Character::Destroy()
{
	Alive = false;
	for (Entity* e : Limbs)
	{
		Limb* a = (Limb*) e;
		a->Fling();
	}
	RespawnCounter = 0;
}

void Character::Respawn()
{
	Alive = true;
	SetVelocity(X, 0);
	SetVelocity(Y, 0);
	for (Entity* e : Limbs)
	{
		Limb* a = (Limb*)e;
		a->Reset();
	}
	SetPosition(X, (((float)rand() / RAND_MAX) - 0.5));
	SetPosition(Y, (((float)rand() / RAND_MAX) - 0.5));
	Health = 3;
}