#include "Framework.h"
#include "Unit.h"

Unit::Unit(Vector3 position, Vector3 size, float rotation)
	:position(position), size(size), rotation(rotation)
{
	animRect = new AnimationRect(position, size);
	animator = new Animator();
}

Unit::~Unit()
{
	SAFE_DELETE(animator);
	SAFE_DELETE(animRect);
}

void Unit::SetHp(uint hp)
{}

void Unit::Attacked(Unit * unit)
{}

void Unit::Dead()
{}

void Unit::Move()
{}

void Unit::UpdatePhysics()
{}

vector<Block*> Unit::UpdateSearchBlock(Block* block)
{
	return vector<Block*>();
}

void Unit::Update()
{}

void Unit::Render()
{}
