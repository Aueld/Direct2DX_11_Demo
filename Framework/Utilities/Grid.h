#pragma once

class Grid
{
public:
    Grid(Vector2 size, float space);
    ~Grid();

    void Update();
    void Render();

private:
    void UpdateWorld();
    void CreateVertexBuffer();
    void CreateIndexBuffer();

private:
    vector<VertexColor> vertices;
    VertexBuffer* vb = nullptr;

    vector<UINT> indices;
    IndexBuffer* ib = nullptr;

    InputLayout* il = nullptr;

    VertexShader* vs = nullptr;
    PixelShader* ps = nullptr;

    WorldBuffer* wb = nullptr;
    Matrix world;

    Color color = Values::Red;
    vector<Vector2> Lines;

    Vector2 size;
    float space;
};