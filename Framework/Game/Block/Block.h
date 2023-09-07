#pragma once

#include "Geomatries/TextureRect.h"

class Block : public TextureRect
{
public:
	Block(Vector3 position, Vector3 size, float rotation);
	~Block();

	void Update();
	void Render();

	BoundingBox* GetBoundingBox() { return box; }

private:
	BoundingBox* box = nullptr;
};