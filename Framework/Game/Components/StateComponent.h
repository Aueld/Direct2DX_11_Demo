#pragma once

#include "StateComponent.h"

class StateComponent
{
public:
	StateComponent();
	~StateComponent();

	enum class StateType
	{
		IDLE = 0,
		WALK,
		RUN,
		RUNATTACK,
		ATTACK,
		HITTED,
		JUMP1,
		JUMP2,
		FLY_S,
		FLY_I,
		FLY_F,
		FAILLING,
		DEAD,
	};

public:
	void SetIdle() { currentState = StateType::IDLE; }
	void SetWalk() { currentState = StateType::WALK; }
	void SetRun() { currentState = StateType::RUN; }
	void SetRunAttack() { currentState = StateType::RUNATTACK; }
	void SetAttack() { currentState = StateType::ATTACK; }
	void SetHitted() { currentState = StateType::HITTED; }
	void SetJump1() { currentState = StateType::JUMP1; }
	void SetJump2() { currentState = StateType::JUMP2; }
	void SetFly_S() { currentState = StateType::FLY_S; }
	void SetFly_I() { currentState = StateType::FLY_I; }
	void SetFly_F() { currentState = StateType::FLY_F; }
	void SetFailling() { currentState = StateType::FAILLING; }
	void SetDead()	 { currentState = StateType::DEAD; }

	bool IsIdle() { return currentState == StateType::IDLE; }
	bool IsWalk() { return currentState == StateType::WALK; }
	bool IsRun() { return currentState == StateType::RUN; }
	bool IsRunAttack() { return currentState == StateType::RUNATTACK; }
	bool IsAttack() { return currentState == StateType::ATTACK; }
	bool IsHitted() { return currentState == StateType::HITTED; }
	bool IsJump1() { return currentState == StateType::JUMP1; }
	bool IsJump2() { return currentState == StateType::JUMP2; }
	bool IsFly_S() { return currentState == StateType::FLY_S; }
	bool IsFly_I() { return currentState == StateType::FLY_I; }
	bool IsFly_F() { return currentState == StateType::FLY_F; }
	bool IsFailling() { return currentState == StateType::FAILLING; }
	bool IsDead()	{ return currentState == StateType::DEAD; }

private:
	StateType currentState = StateType::IDLE;
};