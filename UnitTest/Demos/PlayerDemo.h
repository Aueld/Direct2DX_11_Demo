#pragma once

#include "Game/Unit/Unit.h"
#include "Game/Block/Block.h"

class PlayerDemo : public IObject
{
public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;

private:
	class Movement* movement = nullptr;
	class Player* player = nullptr;

	vector<Unit*> units;
	vector<Block*> blocks;

	class TextureRect* map = nullptr;

	class HUD* hud = nullptr;

};