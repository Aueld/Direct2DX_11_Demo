#include "stdafx.h"
#include "CollisionDemo.h"

#include "Geomatries/Rect.h"

void CollisionDemo::Init()
{
	r1 = new Rect({ 100, 100, 0 }, { 100, 100, 1 }, .0f);
	r2 = new Rect({ 300, 300, 0 }, { 100, 100, 1 }, .0f);
}

void CollisionDemo::Destroy()
{
	SAFE_DELETE(r1);
	SAFE_DELETE(r2);
}

void CollisionDemo::Update()
{
	r1->Update();
	r1->Move();
	
	r2->Update();

	if (BoundingBox::AABB(r1->GetCollision(), r2->GetCollision()) == true ||
		BoundingBox::OBB (r1->GetCollision(), r2->GetCollision()) == true)
		r2->SetColor(Values::Magenta);
	else
		r2->SetColor(Values::Red);
}

void CollisionDemo::Render()
{
	r1->Render();
	r2->Render();
}

void CollisionDemo::PostRender()
{
}

void CollisionDemo::GUI()
{
	r2->GUI();
}
