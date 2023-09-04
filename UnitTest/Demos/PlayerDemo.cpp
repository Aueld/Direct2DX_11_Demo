#include "stdafx.h"
#include "PlayerDemo.h"

#include "Game/Character/Player.h"
#include "Game/UI/HUD.h"

void PlayerDemo::Init()
{
	player = new Player({ 100, 100, 0 }, { 100, 100, 1 });
	hud = new HUD();
	Sounds::Get()->AddSound("BGM", SoundPath + L"ex.mp3");
	Sounds::Get()->Play("BGM", 0.1f);
}

void PlayerDemo::Destroy()
{
	//SAFE_DELETE(hud);
	SAFE_DELETE(map);
	SAFE_DELETE(player);
}

void PlayerDemo::Update()
{
	player->Update();
	//hud->Update();

	auto key = Keyboard::Get();

	if (key->Down(VK_F2))
	{
		if (!Sounds::Get()->IsPaused("BGM"))
			Sounds::Get()->Pause("BGM");
		else
			Sounds::Get()->Resume("BGM");
	}
}

void PlayerDemo::Render()
{
	player->Render();
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
