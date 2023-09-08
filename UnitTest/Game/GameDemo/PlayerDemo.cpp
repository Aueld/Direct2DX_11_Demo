#include "stdafx.h"
#include "PlayerDemo.h"

#include "Game/Unit/Unit.h"
#include "Game/Block/Block.h"
#include "Game/Character/Player.h"
#include "Game/UI/HUD.h"

void PlayerDemo::Init()
{
	units.clear();
	blocks.clear();

	player = new Player({ 100, 100, 0 }, { 100, 100, 1 }, 0.0f);

	blocks.push_back(new Block({ WinMaxWidth / 2, 0, 0 }, { 2000, 100, 1 }, 0.0f));
	units.push_back(player);

	//hud = new HUD();
	//Sounds::Get()->AddSound("BGM", SoundPath + L"ex.mp3");
	//Sounds::Get()->Play("BGM", 0.1f);
}

void PlayerDemo::Destroy()
{
	//SAFE_DELETE(hud);
	//SAFE_DELETE(map);

	for (auto block : blocks)
		SAFE_DELETE(block);

	SAFE_DELETE(player);
}

void PlayerDemo::Update()
{
	player->Update();

	for (auto block : blocks)
	{
		//player->CHECK_AABB(block);
		block->Update();
	}

	//hud->Update();

	//auto key = Keyboard::Get();

	//if (key->Down(VK_F2))
	//{
	//	if (!Sounds::Get()->IsPaused("BGM"))
	//		Sounds::Get()->Pause("BGM");
	//	else
	//		Sounds::Get()->Resume("BGM");
	//}
}

void PlayerDemo::Render()
{
	player->Render();

	for (auto block : blocks)
		block->Render();
	//hud->Render();
}

void PlayerDemo::PostRender()
{
	//TXT->BeginDraw();
	//{
	//	string str = "Text Å×½ºÆ® ¡Ù¡Ú";
	//	wstring wstr = String::ToWstring(str);
	//	float size = (float)TXT->GetStringWidth(wstr, 20.0f);
	//	Vector2 pos = Vector2(500, 500);
	//	TXT->RenderText(wstr, pos, Color(1, 1, 1, 1), (float)size, false);
	//}
	//
	//TXT->EndDraw();
}

void PlayerDemo::GUI()
{
}
