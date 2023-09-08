#pragma once

#include "Game/Block/Block.h"
#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"

class Unit
{
public:
	Unit(Vector3 position, Vector3 size, float rotation);
	~Unit();

	virtual void SetHp(uint hp);
	virtual void Attacked(Unit* unit);
	virtual void Dead();

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void Move();
	virtual void UpdatePhysics();

	void SetPosition(Vector3& position) { this->position = position; }
	void OnGround(bool gound) { onGround = gound; }

	BoundingBox* GetBoundingBox() { return box; }
	Vector3 GetPosition() { return position; }
	Vector3 GetSize() { return size; }
	float GetRotation() { return rotation; }

protected:
	uint HP;
	uint Attack;

	Vector3 position;
	Vector3 size;
	float rotation;

	// Physics
	Vector3 velocity;
	
	float velocityMax = 20.0f;
	float speed;

	bool onGround = true;

	BoundingBox* box = nullptr;
	BoundingBox* plat = nullptr;
	BoundingBox* attackRange = nullptr;

	AnimationRect* animRect = nullptr;
	Animator* animator = nullptr;
};