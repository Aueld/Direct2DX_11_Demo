#include "stdafx.h"
#include "TextureDemo.h"

#include "Geomatries/TextureRect.h"

void TextureDemo::Init()
{
	tr = new TextureRect(Vector3(640, 360, 0), Vector3(500, 500, 1), 0.0f, TexturePath + L"error.jpg");
}

void TextureDemo::Destroy()
{
	SAFE_DELETE(tr);

	for (TextureRect* t : trs)
		SAFE_DELETE(t);
}

void TextureDemo::Update()
{
	srand((UINT)time(NULL));

	tr->Update();

	if (Mouse::Get()->Down(0))
		trs.push_back(new TextureRect(Mouse::Get()->GetPosition(), Vector3(500, 500, 1), 0.0f, TexturePath + L"error.jpg"));
	for (TextureRect* t : trs)
		t->Update();
}

void TextureDemo::Render()
{
	tr->Render();

	for (TextureRect* t : trs)
		t->Render();
}

void TextureDemo::PostRender()
{
}

void TextureDemo::GUI()
{
}
