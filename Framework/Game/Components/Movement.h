#pragma once

#include "Game/Unit/Unit.h"
#include "Game/Block/Block.h"

class Rigid
{
public:
	
private:
	Vector2 position;
	Vector2 velocity;
};

class Movement
{
public:
	Movement(vector<Unit*> units, vector<Block*> blocks);
	~Movement();

	void Update();
	void Gravity(Unit* unit, float delta);

private:
	vector<Unit*> units;
	vector<Block*> blocks;

	Vector2 speed = Values::ZeroVec2;
	
	float gravity = 2.8f;
	float jumpVelocity = 100.0f;
};
