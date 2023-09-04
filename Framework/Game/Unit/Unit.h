#pragma once

#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"
#include "Game/Components/Movement.h"

class Unit
{
public:
	Unit(Vector3 position, Vector3 size);
	~Unit();

	virtual void SetHp(uint hp);
	virtual void Attacked(Unit* unit);
	virtual void Dead();

	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	uint HP;
	uint Attack;

	BoundingBox* box = nullptr;
	BoundingBox* plat = nullptr;
	BoundingBox* attackRange = nullptr;

	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
};

