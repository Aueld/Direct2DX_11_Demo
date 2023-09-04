#pragma once

typedef D3DXVECTOR4 Vector4;

class TileMapBuffer : public ShaderBuffer
{
public:
	TileMapBuffer() : ShaderBuffer(&data, sizeof(Data)) {}

	void SetMPos(Vector3 position)
	{
		data.mPos = Vector4(position.x, position.y, position.z, 0);
	}

	struct Data
	{
		Vector4 mPos;
	};

private:
	Data data;
};

class TileMap
{
public:
	TileMap(uint width, uint height, uint spacing);
	~TileMap();

	void Update();
	void Render();
	void GUI();

	bool GetPickLocation();

	void MapVertexBuffer();
	void UnmapVertexBuffer();

private:
	uint width = 0;
	uint height = 0;
	uint spacing = 0;

	uint vertexCount = 0;
	uint indexCount = 0;

	vector<VertexTextureColor> vertices;
	VertexBuffer* vb = nullptr;

	vector<uint> indices;
	IndexBuffer* ib = nullptr;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	InputLayout* il = nullptr;

	Matrix world = Values::Identity;
	WorldBuffer* wb = nullptr;

	ID3D11ShaderResourceView* srv = nullptr;

	ID3D11RasterizerState* wire[2];
	Vector3 pickedLocation = Vector3(-1, -1, -1);

	D3D11_MAPPED_SUBRESOURCE subResource;

	TileMapBuffer* tmb = nullptr;
};