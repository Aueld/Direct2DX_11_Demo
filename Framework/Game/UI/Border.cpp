#include "Framework.h"
#include "Border.h"

Border::Border(Vector3 position, Vector3 size, float rotation, Color color)
{
	Init(position, size, rotation, color);
}

Border::Border(Vector3 position, Vector3 size, float rotation, wstring path)
{
	Init(position, size, rotation, path);
}

Border::Border(Vector3 position, Vector3 size, float rotation)
{
	Init(position, size, rotation);
}

Border::~Border()
{

}