#pragma once

class Movement
{
public:
	Movement();
	~Movement();

	void Update(Vector3* position);

private:
	Vector2 speed = Values::ZeroVec2;
	
	float gravity = 9.8f;
	float jumpVelocity = 100.0f;
};