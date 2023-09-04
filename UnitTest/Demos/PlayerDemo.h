#pragma once

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
	class Player* player = nullptr;
	class TextureRect* map = nullptr;

	class HUD* hud = nullptr;

	vector<BoundingBox*> walls;
};