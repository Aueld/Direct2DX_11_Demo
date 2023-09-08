#include "stdafx.h"
#include "AnimationDemo.h"

#include "Geomatries/TextureRect.h"
#include "Geomatries/AnimationRect.h"

void AnimationDemo::Init()
{
	ar = new AnimationRect({ 720,360.,0 }, { 200, 200, 1 });
}

void AnimationDemo::Destroy()
{
	SAFE_DELETE(ar);
}

void AnimationDemo::Update()
{
	//ar->Update();
}

void AnimationDemo::Render()
{
	//ar->Render();
}

void AnimationDemo::PostRender()
{
}

void AnimationDemo::GUI()
{
}
