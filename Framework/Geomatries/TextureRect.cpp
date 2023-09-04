#include "Framework.h"
#include "TextureRect.h"

TextureRect::TextureRect(Vector3 position, Vector3 size, float rotation, wstring path)
	:position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = verticesLocalPosition[0] = D3DXVECTOR3(-0.5f, -0.5f, -0.0f);
		vertices[1].position = verticesLocalPosition[1] = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		vertices[2].position = verticesLocalPosition[2] = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[3].position = verticesLocalPosition[3] = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);

		vertices[0].uv = D3DXVECTOR2(0, 1);
		vertices[1].uv = D3DXVECTOR2(1, 0);
		vertices[2].uv = D3DXVECTOR2(1, 1);
		vertices[3].uv = D3DXVECTOR2(0, 0);
	}

	//VertexBuffer
	{
		vb = new VertexBuffer();
		vb->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//IndexBuffer
	{
		indices = { 0, 1, 2, 0, 3, 1 };

		ib = new IndexBuffer();
		ib->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//VertexShader
	{
		vs = new VertexShader();
		vs->Create(ShaderPath + L"VertexTexture.hlsl", "VS");
	}

	//PixelShader
	{
		ps = new PixelShader();
		ps->Create(ShaderPath + L"VertexTexture.hlsl", "PS");
	}

	//InputLayout
	{
		il = new InputLayout();
		il->Create(VertexTexture::descs, VertexTexture::count, vs->GetBlob());
	}

	//WorldBuffer
	{
		wb = new WorldBuffer();
	}

	// SRV
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			path.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		CHECK(hr);

		SRV::Get()->AddSRV(String::ToString(path), srv);
	}
}

TextureRect::TextureRect(Vector3 position, Vector3 size, float rotation)
	: position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = verticesLocalPosition[0] = D3DXVECTOR3(-0.5f, -0.5f, -0.0f);
		vertices[1].position = verticesLocalPosition[1] = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		vertices[2].position = verticesLocalPosition[2] = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[3].position = verticesLocalPosition[3] = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);

		vertices[0].uv = D3DXVECTOR2(0, 1);
		vertices[1].uv = D3DXVECTOR2(1, 0);
		vertices[2].uv = D3DXVECTOR2(1, 1);
		vertices[3].uv = D3DXVECTOR2(0, 0);
	}

	//VertexBuffer
	{
		vb = new VertexBuffer();
		vb->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//IndexBuffer
	{
		indices = { 0, 1, 2, 0, 3, 1 };

		ib = new IndexBuffer();
		ib->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//VertexShader
	{
		vs = new VertexShader();
		vs->Create(ShaderPath + L"VertexTexture.hlsl", "VS");
	}

	//PixelShader
	{
		ps = new PixelShader();
		ps->Create(ShaderPath + L"VertexTexture.hlsl", "PS");
	}

	//InputLayout
	{
		il = new InputLayout();
		il->Create(VertexTexture::descs, VertexTexture::count, vs->GetBlob());
	}

	//WorldBuffer
	{
		wb = new WorldBuffer();
	}
}

TextureRect::~TextureRect()
{
	SAFE_DELETE(vb);
	SAFE_DELETE(ib);
	SAFE_DELETE(vs);
	SAFE_DELETE(ps);
	SAFE_DELETE(il);
	SAFE_DELETE(wb);
}

void TextureRect::Move()
{
	//if (Keyboard::Get()->Press('W'))
	//	position.y += 100 * Time::Delta();
	//else if (Keyboard::Get()->Press('S'))
	//	position.y -= 100 * Time::Delta();

	//if (Keyboard::Get()->Press('D'))
	//	position.x += 100 * Time::Delta();
	//else if (Keyboard::Get()->Press('A'))
	//	position.x -= 100 * Time::Delta();
}

void TextureRect::MapVertexBuffer()
{
	DC->Map(vb->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TextureRect::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	DC->Unmap(vb->GetResource(), 0);
}

void TextureRect::SetShader(wstring shaderpath)
{
	vs->Clear();
	ps->Clear();

	vs->Create(shaderpath, "VS");
	ps->Create(shaderpath, "PS");
}

void TextureRect::Update()
{
	UpdateWorld();
	//Move();
}

void TextureRect::UpdateWorld()
{
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixScaling(&S, size.x, size.y, size.z);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));

	world = S * R * T;
	wb->SetWorld(world);
}

void TextureRect::Render()
{
	vb->SetIA();
	ib->SetIA();
	il->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vs->SetShader();
	ps->SetShader();
	wb->SetVSBuffer(0);

	// PS에서 사용할 SRV 설정
	DC->PSSetShaderResources(0, 1, &srv);

	DC->DrawIndexed(ib->GetCount(), 0, 0);
}

void TextureRect::GUI()
{
}

/*
uv 좌표
- 텍스처 매핑에서 사용되는 2D 좌표계
- uv 좌표는 텍스처의 가로축(u)와 세로축(v)을 나타냄
- (0, 0)은 텍스처의 왼쪽 위 모서리에 해당
- (1, 1)은 텍스처의 오른쪽 아래 모서리에 해당
*/