#pragma once

enum EDrawingMode
{
	Normal,
	RedCell
};

enum ColorChoice
{
	Default
};

class IndexNumBuffer : public ShaderBuffer
{
public:
	IndexNumBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		data.index = 0;
	}

	void SetIndex(uint index) { data.index = index; }

	struct Data
	{
		uint index;
		Vector3 dummy;
	};

private:
	Data data;
};

class TMap
{
public:
	TMap(uint width, uint height, uint spacing);
	~TMap();

	void Update();
	void Render();
	void GUI();

	void GenerateTileMap();
	void MapVertexBuffer();
	void UnmapVertexBuffer();

	void Save(string path);
	void Load(string path);

	class Tile* GetTile(Vector3 mPos);

private:
	uint width = 0;
	uint height = 0;
	uint spacing = 0;

	uint vectexCount = 0;
	uint indexCount = 0;

	vector<VertexTile> vertices;
	VertexBuffer* vb = nullptr;
	D3D11_MAPPED_SUBRESOURCE subResource;

	vector<uint> indices;
	IndexBuffer* ib = nullptr;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	InputLayout* il = nullptr;

	Matrix world = Values::Identity;
	WorldBuffer* wb = nullptr;

	IndexNumBuffer* inb = nullptr;

	class Tile** tiles = nullptr;

	EDrawingMode  mode = Normal;
	ColorChoice choice = Default;
};