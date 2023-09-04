#pragma once

#include "Utilities/UI.h"

class ProgressBar : public UI
{
public:
	ProgressBar(Vector3 position, Vector3 size, float rotation, Color color, FillType type = UI::LEFT_TO_RIGHT);
	ProgressBar(Vector3 position, Vector3 size, float rotation, wstring path, FillType type = UI::LEFT_TO_RIGHT);
	ProgressBar(Vector3 position, Vector3 size, float rotation, FillType type = UI::LEFT_TO_RIGHT);
	~ProgressBar();

	virtual void Update() override;
	virtual void Render() override;
	
	void UpdateProgressBar(float percent);

	FillType type = NONE;

private:

};