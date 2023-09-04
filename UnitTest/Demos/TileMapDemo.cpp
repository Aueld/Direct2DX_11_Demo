#include "stdafx.h"
#include "TileMapDemo.h"

#include "Geomatries/Tile/TMap.h"

void TileMapDemo::Init()
{
	uint spacing = 40;
	uint width = WinMaxWidth / spacing;
	uint height = WinMaxHeight / spacing;
	tm = new TMap(width, height, spacing);
}

void TileMapDemo::Destroy()
{
	SAFE_DELETE(tm);
}

void TileMapDemo::Update()
{
	string s;
	if (KEYBOARD->Press(VK_LCONTROL) && KEYBOARD->Press('S'))
	{
		cin >> s;
		tm->Save(TilePath + s);
	}
	
	if (KEYBOARD->Press(VK_LCONTROL) && KEYBOARD->Press('L'))
	{
		cin >> s;
		tm->Load(TilePath + s);
	}

	tm->Update();
}

void TileMapDemo::Render()
{
	tm->Render();
}

void TileMapDemo::PostRender()
{
}

void TileMapDemo::GUI()
{
	tm->GUI();
}
