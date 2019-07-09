#pragma once
#include "Entity.h"
class Character :
	public Entity
{
public:

	Character(const float* _Position, bool _PlayerControlled)
	{
		SetStart(_Position);
		SetController(_PlayerControlled);
		Name = "PLAYER";
		MovementSpeed = 0.005f;
	}

	Character* target;
	std::vector<Entity*> Limbs;
	float Range = 0.115f;
	bool Alive = true;
	int Damage = 1;
	int Modifier = 1;
	int Health = 4;
	bool AttackCoolDown = false;
	int AICooldown;
	int CooldownTime = 10;

	const float* tPosition;

	float angle;

	int RespawnCounter;

	void Start()
	{
		GetLimbs();

		if (GetController() == COMPUTER)
		{
			AICooldown = CooldownTime;
		}
	}

	void Update()
	{
		if (GetController() == COMPUTER)
		{
			MovementSpeed = 0.004f;
			GetTarget();
			if (target != NULL)
			{
				tPosition = target->GetPosition();
				angle = (float)std::atan2(Position[1] - tPosition[1], Position[0] - tPosition[0]);
				SetVelocity(X, MovementSpeed * -cos(angle));
				SetVelocity(Y, MovementSpeed * -sin(angle));

				if (Velocity[0] > 0 && Velocity[0] > Velocity[1] && Velocity[0] > -Velocity[1]) Facing = RIGHT;
				if (Velocity[0] < 0 && Velocity[0] < Velocity[1] && Velocity[0] < -Velocity[1]) Facing = LEFT;
				if (Velocity[1] > 0 && Velocity[1] > Velocity[0] && Velocity[1] > -Velocity[0]) Facing = UP;
				if (Velocity[1] < 0 && Velocity[1] > Velocity[0] && Velocity[1] < -Velocity[0]) Facing = DOWN;

				if (GetDistance(tPosition) <= Range - 0.015f)
				{
					SetVelocity(X, 0);
					SetVelocity(Y, 0);
				}

				if (AICooldown <= 0 && Alive)
				{
					AttackCoolDown = false;
					AICooldown = CooldownTime;
					Attack();
				}

				if (!target->Alive) target = NULL;
			}

			AICooldown--;
		}

		RespawnCounter++;

		if (RespawnCounter == 120 && !Alive) Respawn();

		if (Health <= 0 && Alive) Destroy();
	}

	void Destroy();

	void Attack();
	void GetTarget();
	void GetLimbs();
	
	void Respawn();

	//Get the distance to another entity
	float GetDistance(const float* target)
	{
		float a, b, c, d;
		a = Position[0] - target[0];
		b = Position[1] - target[1];
		c = Position[2] - target[2];

		a *= a;
		b *= b;
		c *= c;
		d = a + b;

		return sqrt(d);
	}

	void TakeDamage(int Amount)
	{
		if(Alive)
		Health -= (Amount * Modifier);
	}

	void KeyDown(char key)
	{
		if (Alive)
		switch (key)
		{
		case (KEY_W) :
			SetVelocity(Y, MovementSpeed);
			Facing = UP;
			break;
		case (KEY_A) :
			SetVelocity(X, -MovementSpeed);
			Facing = LEFT;
			break;
		case (KEY_S) :
			SetVelocity(Y, -MovementSpeed);
			Facing = DOWN;
			break;
		case (KEY_D) :
			SetVelocity(X, MovementSpeed);
			Facing = RIGHT;
			break;
		case (M_LEFT) :
		case (KEY_SPACE):
			Attack();
			break;
		}
	}

	void KeyUp(char key)
	{
		if (Alive)
		switch (key)
		{
		case (KEY_W) :
			SetVelocity(Y, 0);
			break;
		case (KEY_A) :
			SetVelocity(X, 0);
			break;
		case (KEY_S) :
			SetVelocity(Y, 0);
			break;
		case (KEY_D) :
			SetVelocity(X, 0);
			break;
		case(M_LEFT) :
		case (KEY_SPACE) :
			AttackCoolDown = false;
		}
	}
};

