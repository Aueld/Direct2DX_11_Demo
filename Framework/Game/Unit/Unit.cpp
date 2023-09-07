#include "Framework.h"
#include "Unit.h"

Unit::Unit(Vector3 position, Vector3 size)
	:position(position), size(size)
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

void Unit::Update()
{}

void Unit::Render()
{}
