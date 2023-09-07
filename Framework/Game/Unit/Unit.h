#pragma once

#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"

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

	void SetPosition(Vector3& position) { this->position = position; animRect->SetPosition(position); }

	BoundingBox* GetBoundingBox() { return box; }
	Vector3 GetPosition() { return animRect->GetPosition(); }
	Vector3 GetSize() { return animRect->GetSize(); }
	float GetRotation() { return animRect->GetRotation(); }

protected:
	uint HP;
	uint Attack;

	Vector3 position;
	Vector3 size;

	BoundingBox* box = nullptr;
	BoundingBox* plat = nullptr;
	BoundingBox* attackRange = nullptr;

	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
};

