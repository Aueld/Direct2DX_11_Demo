#include "Framework.h"
#include "Character.h"

Character::Character(Vector3 position, Vector3 size)
{
	animRect = new AnimationRect(position, size);
	animator = new Animator();
}

Character::~Character()
{
	SAFE_DELETE(animRect);
	SAFE_DELETE(animator);
}
