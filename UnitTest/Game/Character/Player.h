#pragma once

#include "Game/Unit/Actor/Character.h"

class Player : public Character
{
public:
	Player(Vector3 position, Vector3 size);
	~Player();

	virtual void Update() override;
	virtual void Render() override;

	void SetAnimator();

private:
	//BoundingBox* collision = nullptr;
	class StateComponent* stateComponent = nullptr;
};