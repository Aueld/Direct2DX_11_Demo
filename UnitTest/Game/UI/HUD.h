#pragma once

#include "Utilities/UI.h"

class HUD
{
public:
	HUD();
	~HUD();

	void Update();
	void Render();

public:
	class Border* background = nullptr;
	class ProgressBar* HPbar = nullptr;

	float percent = 1.f;
};