#include "Framework.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, Color color, FillType type)
	: type(type)
{
	Init(position, size, rotation, color, type);
}

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, wstring path, FillType type)
	: type(type)
{
	Init(position, size, rotation, path, type);
}

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, FillType type)
	: type(type)
{
	Init(position, size, rotation, type);
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::Update()
{
	UI::Update();
}

void ProgressBar::Render()
{
	UI::Render();
}

void ProgressBar::UpdateProgressBar(float percent)
{
	Vector3 changeSize;

	if(type == LEFT_TO_RIGHT || type == RIGHT_TO_LEFT)
		changeSize = Vector3(originSize.x * percent, originSize.y, originSize.z);
	else
		changeSize = Vector3(originSize.x, originSize.y * percent, originSize.z);

	size = changeSize;
}

