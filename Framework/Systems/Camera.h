#pragma once

class Camera : public SingletonBase<Camera>
{
public:
	friend class SingletonBase<Camera>;

	void Update();
	void Render();

	void Move();
	void UpdateView();

	const Vector3 GetPosition() { return position; }

	// 반드시 Render() 맨 마지막에 넣을것
	void SetPosition(Vector3 position) { this->position = position; }

	void UnProjection(Vector3* out, Vector3 source, Matrix world);

private:
	Camera();
	~Camera();

	VPBuffer* vpb = nullptr;
	Matrix view, proj;

	Vector3 position = Values::ZeroVec3;
	float cameraSpeed = 100.0f;
};