#include "Framework.h"
#include "Movement.h"

Movement::Movement()
{

}

Movement::~Movement()
{

}

void Movement::Update(Vector3& position)
{
	float delta = Time::Delta();
	speed = gravity * delta * 100.0f;

	position -= Vector3(position.x, speed, position.z);
}
