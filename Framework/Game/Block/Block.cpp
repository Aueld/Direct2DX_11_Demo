#include "Framework.h"
#include "Block.h"

Block::Block(Vector3 position, Vector3 size, float rotation)
	: TextureRect(position, size, rotation)
{
	box = new BoundingBox(position, size, rotation, Color(1, 0, 0, 1));
}

Block::~Block()
{
	SAFE_DELETE(box);
}

void Block::Update()
{
	box->Update(position, size, rotation);
}

void Block::Render()
{
	box->Render();
}
