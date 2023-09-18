#pragma once

#include "Game/Unit/Unit.h"
#include "Game/Block/Block.h"
#include "Game/Components/StateComponent.h"
#include "Systems/Time.h"

class Player : public Unit
{
public:
	Player(Vector3 position, Vector3 size, float rotation);
	~Player();

	virtual void Update() override;
	virtual void Render() override;
	
	virtual void Move() override;
	virtual void UpdatePhysics() override;
	virtual vector<Block*> UpdateSearchBlock(Block* block) override;

	void SetTexture2D(wstring imgPath, wstring animClip_R, wstring animClip_L, int count, float frame, bool loop);

	void SetAnimator();

	void CHECK_AABB(Block* box);
	void ShowPlayerAnimation();

private:
	void PlayAnimation(wstring animClip_R, wstring animClip_L);

private:
	class StateComponent* stateComponent = nullptr;
	bool CheckLR = true;	// true == Right, false == Left
	bool attackSlide = false;
	bool isFly = false;
	bool isReadyFly = false;
	bool isFlying = false;

	int oldKey = -1;
	float time = 0.0f;
	float time_Flying = 0.0f;
	float time_Landing = 0.0f;
};