#include "Framework.h"
#include "StateComponent.h"

StateComponent::StateComponent()
{
	currentState = StateType::IDLE;
}

StateComponent::~StateComponent()
{}
