#include "stdafx.h"
#include "RectDemo.h"

#include "Geomatries/Rect.h"
#include "Utilities/Grid.h"

void RectDemo::Init()
{
	rect = new Rect(Vector3(500, 200, 0), Vector3(100, 100, 1), 0.0f);
	grid = new Grid(Vector2(20, 30), 100);
}

void RectDemo::Destroy()
{
	SAFE_DELETE(rect);
}

void RectDemo::Update()
{
	rect->Update();
	grid->Update();
	CAMERA->Move();
}

void RectDemo::Render()
{
	rect->Render();
	grid->Render();
}

void RectDemo::PostRender()
{

}

void RectDemo::GUI()
{
	ImGui::Begin("Rects");
	{
		rect->GUI();
	}
	ImGui::End();
}
