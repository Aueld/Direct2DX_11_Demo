#pragma once

#include "Utilities/UI.h"

class Border : public UI
{
public:
	Border(Vector3 position, Vector3 size, float rotation, Color color);
	Border(Vector3 position, Vector3 size, float rotation, wstring path);
	Border(Vector3 position, Vector3 size, float rotation);
	~Border();

private:

	
};