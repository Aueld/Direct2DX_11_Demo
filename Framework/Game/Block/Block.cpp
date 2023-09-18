#include "Framework.h"
#include "Block.h"

Block::Block(Vector3 position, Vector3 size, float rotation)
	: TextureRect(position, size, rotation)
{
	box = new BoundingBox(position, size, rotation, Color(0.1f, 0.1f, 0.1f, 1));
}

Block::~Block()
{
	SAFE_DELETE(box);
}

void Block::Update()
{
	if (checkBox)
		box->SetColor(Color(1, 1, 0, 0.5f));
	else
		box->SetColor(Color(1, 0, 0, 1.0f));

	box->UpdateCollisionData();
	box->Update(position, size, rotation);
}

void Block::Render()
{
	box->Render();
}

void Block::CheckBlock()
{
	if (!checkBox)
		checkBox = true;
	else
		checkBox = false;
}
