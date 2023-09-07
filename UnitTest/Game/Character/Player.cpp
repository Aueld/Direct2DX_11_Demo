#include "stdafx.h"
#include "Player.h"

Player::Player(Vector3 position, Vector3 size)
	:Unit(position, size)
{
	SetAnimator();

	collision = new BoundingBox(position, size, 0.0f, Color(1, 1, 0, 0.35f));
}

Player::~Player()
{
	SAFE_DELETE(collision);
}

void Player::Update()
{
	animRect->Move();
	animator->Update();
	animRect->Update();

	collision->UpdateCollisionData();
	collision->Update(animRect->GetPosition(), animRect->GetSize(), animRect->GetRotation());
}

void Player::Render()
{
	animRect->Render();
	collision->Render();

	Vector3 cameraPos = Vector3(animRect->GetPosition().x - WinMaxWidth / 2, animRect->GetPosition().y - WinMaxHeight / 2, 0.0f);
	CAMERA->SetPosition(cameraPos);
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