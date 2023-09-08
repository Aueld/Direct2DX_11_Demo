#include "stdafx.h"
#include "Player.h"

Player::Player(Vector3 position, Vector3 size, float rotation)
	:Unit(position, size, rotation)
{
	SetAnimator();

	box = new BoundingBox(position, size, rotation, Color(1, 1, 0, 0.35f));
}

Player::~Player()
{
	SAFE_DELETE(box);
}

void Player::Update()
{
	this->Move();
	animator->Update();
	animRect->Update(position, size, rotation);

	box->UpdateCollisionData();
	box->Update(position, size, rotation);
}

void Player::Render()
{
	animRect->Render();
	box->Render();

	Vector3 cameraPos = Vector3(animRect->GetPosition().x - WinMaxWidth / 2, animRect->GetPosition().y - WinMaxHeight / 2, 0.0f);
	CAMERA->SetPosition(cameraPos);
}

void Player::Move()
{
	speed = 300.0f;

	velocity.x = 0.0f;

	if (Keyboard::Get()->Press('A'))
	{
		velocity.x -= speed * Time::Delta();
		animator->SetCurrentAnimClip(L"RunL");
	}
	else if (Keyboard::Get()->Press('D'))
	{
		velocity.x += speed * Time::Delta();
		animator->SetCurrentAnimClip(L"RunR");
	}
	else
	{
		// IDLE
		//velocity.y = 0.0f;
	}

	if (Keyboard::Get()->Press(VK_SPACE) && onGround)
	{
		velocity.y = +20.0f;
		onGround = false;
	}

	UpdatePhysics();
}

void Player::UpdatePhysics()
{
	if (!onGround)
	{
		// JUMP
		velocity.y -= 0.98f;
		//&& velocity.y > std::abs(velocityMax)
		//velocity.y += ((velocity.y < 0) ? -1.0f : 1.0f);
	}

	position.x += velocity.x;
	position.y += velocity.y;

	if (position.y < 100.0f)
	{
		position.y = 100.0f;
		onGround = true;
	}
}

void Player::SetAnimator()
{
	Texture2D* srcTex = new Texture2D(TexturePath + L"jelda.png");
	Vector2 texSize = Vector2(srcTex->GetWidth(), srcTex->GetHeight());

	AnimationClip* RunD = new AnimationClip(L"RunD", srcTex, 10, Vector2(0, 0), Vector2(texSize.x, texSize.y * 0.25f), 1.0f / 15.0f);
	AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 10, Vector2(0, texSize.y * 0.25f), Vector2(texSize.x, texSize.y * 0.5f), 1.0f / 15.0f);
	AnimationClip* RunU = new AnimationClip(L"RunU", srcTex, 10, Vector2(0, texSize.y * 0.5f), Vector2(texSize.x, texSize.y * 0.75f), 1.0f / 15.0f, true);
	AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 10, Vector2(0, texSize.y * 0.75f), texSize, 1.0f / 15.0f, true);

	animator->SetMoveDClip(RunD);
	animator->SetMoveLClip(RunL);
	animator->SetMoveUClip(RunU);
	animator->SetMoveRClip(RunR);

	animator->PlayMoveL();

	animRect->SetAnimation(animator);

	SAFE_DELETE(srcTex);
}

void Player::CHECK_AABB(Block* box)
{
	//if (BoundingBox::AABB(this->box, box->GetBoundingBox()))
	//{
	//	velocity.y = 0;
	//	position.y = box->GetPosition().y + box->GetSize().y;
	//	onGround = true;
	//}
	//else
		//onGround = false;
}
