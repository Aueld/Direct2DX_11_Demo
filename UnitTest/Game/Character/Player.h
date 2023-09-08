#pragma once

#include "Game/Unit/Unit.h"

class Player : public Unit
{
public:
	Player(Vector3 position, Vector3 size, float rotation);
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	
	virtual void Move() override;
	virtual void UpdatePhysics() override;

	void SetAnimator();

	void CHECK_AABB(Block* box);

private:
	class StateComponent* stateComponent = nullptr;
};