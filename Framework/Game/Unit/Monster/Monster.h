#pragma once

#include "Game/Unit/Unit.h"

class Monster : public Unit
{
public:
	virtual void Update() override;
	virtual void Render() override;

	virtual void Move() override;
	virtual void UpdatePhysics() override;
	virtual vector<Block*> UpdateSearchBlock(Block* block) override;

	void SetAnimator();

	void CHECK_AABB(Block* box);

private:
	class StateComponent* stateComponent = nullptr;
};

