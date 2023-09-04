#include "stdafx.h"
#include "HUD.h"

#include "Geomatries/TextureRect.h"
#include "Game/UI/ProgressBar.h"
#include "Game/UI/Border.h"

HUD::HUD()
{
	background = new Border({ 100,100,0 }, { 100,100,1 }, 0.f, TexturePath + L"error.jpg");
	float width = 0;
	float height = 0;
	HPbar = new ProgressBar({ width, height, 0 }, { 200, 30, 1 }, 0.0f, Color(1, 0, 0, 1), UI::DOWN_TO_UP);
}

HUD::~HUD()
{
	SAFE_DELETE(HPbar);
	SAFE_DELETE(background);
}

void HUD::Update()
{
	if(KEYBOARD->Down(VK_F3))
	{
		HPbar->UpdateProgressBar(percent);
		percent -= 0.1f;
	}

	HPbar->Update();
	background->Update();
}

void HUD::Render()
{
	HPbar->Render();
	background->Render();
}
