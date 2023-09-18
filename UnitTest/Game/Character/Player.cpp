#include "stdafx.h"
#include "Player.h"

Player::Player(Vector3 position, Vector3 size, float rotation)
	:Unit(position, size, rotation)
{
	SetAnimator();

	box = new BoundingBox(position, size, rotation, Color(1, 1, 0, 0.35f));
	searchBox = new BoundingBox(position, size * 10.0f, rotation, Color(0, 1, 1, 0.15f));
	stateComponent = new StateComponent();

	speed = 300.0f;

	velocity.y = 0.0f;
}

Player::~Player()
{
	SAFE_DELETE(stateComponent);
	SAFE_DELETE(searchBox);
	SAFE_DELETE(box);
}

void Player::Update()
{
	Move();
	animator->Update();
	animRect->Update(position, size, rotation);

	box->UpdateCollisionData();
	box->Update(position, size, rotation);

	searchBox->UpdateCollisionData();
	searchBox->Update(position, size * 10.0f, rotation);
}

void Player::Render()
{
	animRect->Render();
	
	ShowPlayerAnimation();

	auto key = Keyboard::Get();

	if (key->Down(VK_F2))
	{
		if (!isColliderRender)
			isColliderRender = true;
		else
			isColliderRender = false;
	}

	// Collider Render
	if (isColliderRender)
	{
		box->Render();
		searchBox->Render();
	}

	Vector3 cameraPos = Vector3(GetPosition().x - WinMaxWidth / 2, GetPosition().y - WinMaxHeight / 2, 0.0f);
	CAMERA->SetPosition(cameraPos);
}

void Player::Move()
{
	// IDLE
	velocity.x = 0.0f;

	time = Time::Delta();
	
	// 커맨드, 키변화가 생기고 일정 시간 경과
	if (oldKey != -1)
	{
		dTimer += time;

		if (dTimer > 0.5f)
		{
			oldKey = -1;
			dTimer = 0.0f;
		}
	}

	// 대쉬 공격
	if (attackSlide)
	{
		if (animationTime < 0.0f)
		{
			animationTime = 0.0f;
			attackSlide = false;
			stateComponent->SetIdle();
		}

		if (!CheckLR)
		{
			velocity.x -= speed * 3.0f * time;
		}
		else
		{
			velocity.x += speed * 3.0f * time;
		}
	}

	// [J] Action
	if (Keyboard::Get()->Down('J'))
	{
		if (onGround)
		{

		}
		else if (isDash && onGround)
		{
			animationTime = 0.3f;
			attackSlide = true;
			stateComponent->SetRunAttack();
		}
		else if (isDoubleJump && isFly)
		{
			isFly = false;
		}
	}

	// No AnimationTime MOVE [A, D, SPACE] Action
	if (animationTime < 0.0f)
	{
		// [A] Press
		if (Keyboard::Get()->Press('A'))
		{
			if (oldKey == 0)
				isDash = true;

			// 방향 전환이 가능한 경우
			if (isFly || !isDoubleJump)
				CheckLR = false;

			if (!isDash)
			{
				velocity.x -= speed * time;

				if (!isJump)
					stateComponent->SetWalk();
			}
			else
			{
				velocity.x -= speed * 1.5f * time;

				if (!isJump)
					stateComponent->SetRun();
			}
		}

		// [D] Press
		else if (Keyboard::Get()->Press('D'))
		{
			if (oldKey == 1)
				isDash = true;

			// 방향전환이 가능한 경우
			if (isFly || !isDoubleJump)
				CheckLR = true;

			if (!isDash)
			{
				velocity.x += speed * time;

				if (!isJump)
					stateComponent->SetWalk();
			}
			else
			{
				velocity.x += speed * 1.5f * time;

				if (!isJump)
					stateComponent->SetRun();
			}
		}

		// [A] UP
		if (Keyboard::Get()->Up('A'))
		{
			oldKey = 0;

			isDash = false;

			if (!isJump)
				stateComponent->SetIdle();
		}

		// [D] UP
		else if (Keyboard::Get()->Up('D'))
		{
			oldKey = 1;

			isDash = false;

			if (!isJump)
				stateComponent->SetIdle();
		}

		// actionTime 동안 Fly_S 애니메이션 재생
		if (time_Flying < 0.0f)
		{
			// Fly_S 동작이 끝나고 추가 입력이 없을때
			if (isFly && !isFlying)
			{
				isFlying = true;
				stateComponent->SetFly_F();
			}

			// 추가 입력시 애니메이션 변환
			// FLYING
			if (Keyboard::Get()->Press(VK_SPACE) && isFly)
			{
				stateComponent->SetFly_I();
				velocity.y = +5.0f;
			}
			else if (Keyboard::Get()->Up(VK_SPACE) && isFly)
			{
				stateComponent->SetFly_F();
			}
		}
		else
		{
			time_Flying -= time;
		}

		// JUMP
		if (Keyboard::Get()->Press(VK_SPACE) && onGround)
		{
			isJump = true;

			stateComponent->SetJump1();
			velocity.y = +20.0f;
			onGround = false;
		}

		// JUMP 2
		if (Keyboard::Get()->Down(VK_SPACE) && isJump && !isDoubleJump)
		{
			isDoubleJump = true;

			stateComponent->SetJump2();
			velocity.y = +20.0f;
		}

		// FLY
		else if (Keyboard::Get()->Down(VK_SPACE) && isJump && isDoubleJump && !isFly)
		{
			isFly = true;

			stateComponent->SetFly_S();
			time_Flying = 0.1f;
			velocity.y = +5.0f;
		}
		else if (Keyboard::Get()->Down(VK_SPACE) && isReadyFly && !isFly)
		{
			isReadyFly = false;

			isJump = true;
			isDoubleJump = true;
			isFly = true;

			stateComponent->SetFly_S();
			time_Flying = 0.1f;
			velocity.y = +5.0f;
		}
	}
	else
	{
		animationTime -= time;
	}

	UpdatePhysics();
}

void Player::UpdatePhysics()
{
	if (!onGround)
	{
		if (isFly)
			velocity.y -= 0.28f;
		else
			velocity.y -= 0.98f;
	}

	// 가속도 최대 제한
	if (std::abs(velocity.x) > velocityMax)
		velocity.x = velocity.x > 0 ? velocityMax : -velocityMax;
	if (std::abs(velocity.y) > velocityMax)
		velocity.y = velocity.y > 0 ? velocityMax : -velocityMax;

	position.x += velocity.x;
	position.y += velocity.y;
}

vector<Block*> Player::UpdateSearchBlock(Block* block)
{
	vector<Block*> tempVecBlock;

	tempVecBlock.clear();
	
	if (BoundingBox::AABB(searchBox, block->GetBoundingBox()))
	{
		block->CheckBlock();
		tempVecBlock.push_back(block);
	}

	if (!BoundingBox::AABB(searchBox, block->GetBoundingBox()))
	{
		block->CheckBlock();
	}

	return tempVecBlock;
}

void Player::SetTexture2D(wstring imgPath, wstring animClip_R, wstring animClip_L, int count, float frame, bool loop)
{
	Texture2D* srcTex = new Texture2D(CharacterPath + imgPath);
	Vector2 texSize = Vector2(srcTex->GetWidth(), srcTex->GetHeight());

	AnimationClip* anim_R = new AnimationClip(animClip_R, srcTex, count, Vector2(0, 0), Vector2(texSize.x, texSize.y * 0.5f), 1.0f / frame, loop, false);
	AnimationClip* anim_L = new AnimationClip(animClip_L, srcTex, count, Vector2(0, texSize.y * 0.5f), Vector2(texSize.x, texSize.y), 1.0f / frame, loop, true);

	animator->SetClip(anim_R);
	animator->SetClip(anim_L);

	SAFE_DELETE(srcTex);
}

void Player::SetAnimator()
{
	// 이미지 경로, 클립 우측 이름, 클립 좌측 이름, 이미지 수, 프레임, 역행 유무
	SetTexture2D(L"Kir_IDLE.png", L"Idle_R", L"Idle_L", 2, 1.0f, true);
	SetTexture2D(L"Kir_Walk.png", L"Walk_R", L"Walk_L", 10, 15.0f, true);
	SetTexture2D(L"Kir_Jump1.png", L"Jump1_R", L"Jump1_L", 1, 1.0f, false);
	SetTexture2D(L"Kir_Jump2.png", L"Jump2_R", L"Jump2_L", 8, 15.0f, false);
	SetTexture2D(L"Kir_Fly_S.png", L"Fly_S_R", L"Fly_S_L", 5, 15.0f, false);
	SetTexture2D(L"Kir_Fly_I.png", L"Fly_I_R", L"Fly_I_L", 5, 15.0f, true);
	SetTexture2D(L"Kir_Fly_F.png", L"Fly_F_R", L"Fly_F_L", 2, 2.0f, true);
	SetTexture2D(L"Kir_Run.png", L"Run_R", L"Run_L", 8, 15.0f, true);
	SetTexture2D(L"Kir_RunAttack.png", L"RunAttack_R", L"RunAttack_L", 2, 5.0f, false);
	SetTexture2D(L"Kir_HIT.png", L"Hit_R", L"Hit_L", 10, 15.0f, false);


	animator->PlayAnimation();
	animRect->SetAnimation(animator);
}

void Player::CHECK_AABB(Block* box)
{
	// 충돌체가 없을때
	if (box == nullptr)
		return;

	// 충돌체에 닿았을때
	if (BoundingBox::AABB(this->box, box->GetBoundingBox()))
	{
		// 충돌체가 플레이어보다 낮은 위치에 있을때
		if (box->GetPosition().y + box->GetSize().y / 2.0f < position.y)
		{
			//////
			// ///
			// ///
			// 
			// 착지 작업

			//if (isFly)
			//{
			//	time_Landing = 0.5f;
			//	stateComponent->SetHitted();
			//}
			//else if(isJump)
			//{
			//	time_Landing = 0.5f;
			//	stateComponent->SetHitted();
			//}

			// 점프를 하지 않았으나 착지하였을때
			if (oldPlat == nullptr)
				stateComponent->SetIdle();

			oldPlat = box->GetBoundingBox();
			velocity.y = 0;

			position.y = box->GetPosition().y + box->GetSize().y;

			if (isJump || isDoubleJump || isFly)
			{
				isJump = false;
				isDoubleJump = false;
				isFly = false;
				isReadyFly = false;
				isFlying = false;

				stateComponent->SetIdle();

				//if (time_Landing < 0.0f)
				//{
				//	time_Landing = 0.0f;

				//	stateComponent->SetIdle();
				//}
				//else
				//	time_Landing -= Time::Delta();
			}
			onGround = true;
		}
	}

	// 이전 충돌체와 멀어졌을때
	if (!BoundingBox::AABB(this->box, oldPlat))
	{
		if(!isJump && !isFly)
			stateComponent->SetJump1();
		
		isReadyFly = true;
		onGround = false;
		oldPlat = nullptr;
	}
}

void Player::ShowPlayerAnimation()
{
	if (stateComponent->IsIdle())	
		PlayAnimation(L"Idle_R", L"Idle_L");

	if (stateComponent->IsWalk())
		PlayAnimation(L"Walk_R", L"Walk_L");

	if (stateComponent->IsRun())
		PlayAnimation(L"Run_R", L"Run_L");

	if (stateComponent->IsJump1())
		PlayAnimation(L"Jump1_R", L"Jump1_L");

	if (stateComponent->IsJump2())
		PlayAnimation(L"Jump2_R", L"Jump2_L");

	if (stateComponent->IsRunAttack())
		PlayAnimation(L"RunAttack_R", L"RunAttack_L");

	if (stateComponent->IsHitted())
		PlayAnimation(L"Hit_R", L"Hit_L");

	if (stateComponent->IsFly_S())
		PlayAnimation(L"Fly_S_R", L"Fly_S_L");

	if (stateComponent->IsFly_I())
		PlayAnimation(L"Fly_I_R", L"Fly_I_L");

	if (stateComponent->IsFly_F())
		PlayAnimation(L"Fly_F_R", L"Fly_F_L");
}

void Player::PlayAnimation(wstring animClip_R, wstring animClip_L)
{
	if(CheckLR)
		animator->SetCurrentAnimClip(animClip_R);
	else
		animator->SetCurrentAnimClip(animClip_L);
}