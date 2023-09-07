#pragma once

#include "Game/Unit/Unit.h"

class Player : public Unit
{
public:
	Player(Vector3 position, Vector3 size);
	~Player();

	virtual void Update() override;
	virtual void Render() override;

	void SetAnimator();

private:
	BoundingBox* collision = nullptr;
	class StateComponent* stateComponent = nullptr;
};