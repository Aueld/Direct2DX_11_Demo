#include "Framework.h"
#include "Movement.h"

Movement::Movement(vector<Unit*> units, vector<Block*> blocks)
	:units(units), blocks(blocks)
{

}

Movement::~Movement()
{

}

void Movement::Update()
{
	float delta = Time::Delta();

	for (auto* unit : units)
	{
		for (auto* block : blocks)
		{
			if (!BoundingBox::AABB(unit->GetBoundingBox(), block->GetBoundingBox()))
			{
				Gravity(unit, delta);
			}
		}
	}
}

void Movement::Gravity(Unit* unit, float delta)
{
	static const Vector2 gravity = Vector2(0.0f, 9.8f);

	Vector3 pos = unit->GetPosition();
	
	pos -= Vector3(pos.x, pos.y * gravity * delta * 10.0f, pos.z);

	unit->SetPosition(pos);
}

// 유닛 단위와 블록 단위로 충돌 관리