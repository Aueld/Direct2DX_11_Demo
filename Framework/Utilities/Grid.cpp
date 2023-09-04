#include "Framework.h"
#include "Grid.h"

Grid::Grid(Vector2 size, float space)
    : size(size), space(space)
{
    CreateVertexBuffer();
    CreateIndexBuffer();
    
    vs = new VertexShader();
    vs->Create(ShaderPath + L"VertexColor.hlsl", "VS");

    ps = new PixelShader();
    ps->Create(ShaderPath + L"VertexColor.hlsl", "PS");

    il = new InputLayout();
    il->Create(VertexColor::descs, VertexColor::count, vs->GetBlob());

    wb = new WorldBuffer();
}

Grid::~Grid()
{
    SAFE_DELETE(wb);
    SAFE_DELETE(il);
    SAFE_DELETE(ps);
    SAFE_DELETE(vs);
    SAFE_DELETE(ib);
    SAFE_DELETE(vb);
}

void Grid::Update()
{
    UpdateWorld();
}

void Grid::Render()
{
    vb->SetIA();
    ib->SetIA();
    il->SetIA();
    DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    vs->SetShader();
    wb->SetVSBuffer(0);
    
    ps->SetShader();

    DC->DrawIndexed(ib->GetCount(), 0, 0);
}

void Grid::UpdateWorld()
{
    world =
        DXMath::Scaling(Vector3(1, 1, 1)) *
        DXMath::RotationInDegree(0) *
        DXMath::Translation(Vector3(0, 0, 0));

    wb->SetWorld(world);
}

void Grid::CreateVertexBuffer()
{
    uint count = 2 * (size.x + 1) + 2 * (size.y - 1);
    uint lastLineIndex = count - (size.x + 1);

    vertices.assign(count, VertexColor());

    for (uint i = 0; i < count; i++)
    {
        int x = 0;
        if (i <= size.x)
            x = space * i;
        else if (i < lastLineIndex)
        {
            if ((int)size.x % 2 == 0)
                x = i % 2 == 0 ? space * size.x : 0;
            else
                x = i % 2 != 0 ? space * size.x : 0;
        }
        else
            x = space * (i - lastLineIndex);

        int y = 0;
        if (i <= size.x)
            y = 0;
        else if (i < lastLineIndex)
        {
            int temp = round((i - size.x + 0.5f) / 2);
            y = space * temp;
        }
        else
            y = size.y * space;

        vertices[i].position = Vector3(x, y, 0.0f);
        vertices[i].color = Values::Red;
    }

    vb = new VertexBuffer();
    vb->Create(vertices, D3D11_USAGE_DYNAMIC);
}

void Grid::CreateIndexBuffer()
{
    uint count = (size.x + 1) * 2 + (size.y + 1) * 2;
    uint line  = (size.y + 1) * 2;
    uint row   = (size.x + 1) * 2;

    uint lastLineIndex = 2 * (size.x + 1) + 2 * (size.y - 1) - (size.x + 1);

    indices.assign(count , UINT());

    // 가로줄
    for (int i = 0; i < line; i++)
    {
        uint n = 0;

        if (i == 0)
            n = 0;
        else if (i == 1)
            n = size.x;
        else if (i == line - 2)
            n = lastLineIndex;
        else if (i == line - 1)
            n = lastLineIndex + size.x;
        else
            n = size.x + i - 1;

        indices[i] = n;
    }

    // 세로줄
    for (int i = 0; i < row; i++)
    {
        uint n = 0;
        if (i == 0)
            n = 0;
        else if (i == 1)
            n = lastLineIndex;
        else if (i == row - 2)
            n = size.x;
        else if (i == row - 1)
            n = 2 * (size.x + 1) + 2 * (size.y - 1) - 1;
        else
        {
            if (i % 2 == 0)
                n = i / 2;
            else
                n = (i / 2) + lastLineIndex;
        }

        indices[i + line] = n;
    }

    ib = new IndexBuffer();
    ib->Create(indices, D3D11_USAGE_IMMUTABLE);
}
