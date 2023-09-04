#pragma once

#include "StateComponent.h"

class StateComponent
{
public:
	enum class StateType
	{
		IDLE = 0,
		ATTACK,
		HITTED,
		JUMP,
		DEAD,
	};

public:
	void SetIdle()	 { currentState = StateType::IDLE; }
	void SetAttack() { currentState = StateType::ATTACK; }
	void SetHitted() { currentState = StateType::HITTED; }
	void SetJump()	 { currentState = StateType::JUMP; }
	void SetDead()	 { currentState = StateType::DEAD; }

	bool IsIdle()	{ return currentState == StateType::IDLE; }
	bool IsAttack() { return currentState == StateType::ATTACK; }
	bool IsHitted() { return currentState == StateType::HITTED; }
	bool IsJump()	{ return currentState == StateType::JUMP; }
	bool IsDead()	{ return currentState == StateType::DEAD; }

private:
	StateType currentState = StateType::IDLE;
};