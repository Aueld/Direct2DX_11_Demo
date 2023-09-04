#include "Framework.h"
#include "Rect.h"

Rect::Rect(Vector3 position, Vector3 size, float rotation)
	:position(position), size(size), rotation(rotation)
{
	{
		// vertices
		{
			vertices.assign(4, VertexColor());

			vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
			vertices[0].color = Color(1, 0, 0, 1);

			vertices[1].position = Vector3(0.5f, 0.5f, 0.0f);
			vertices[1].color = Color(1, 0, 0, 1);

			vertices[2].position = Vector3(0.5f, -0.5f, 0.0f);
			vertices[2].color = Color(1, 0, 0, 1);

			vertices[3].position = Vector3(-0.5f, 0.5f, 0.0f);
			vertices[3].color = Color(1, 0, 0, 1);
		}

		// Vertex Buffer
		{
			vb = new VertexBuffer();
			vb->Create(vertices, D3D11_USAGE_DYNAMIC);
		}

		// Index Buffer
		{
			indices = { 0, 1, 2, 0, 3, 1 };

			ib = new IndexBuffer();
			ib->Create(indices, D3D11_USAGE_IMMUTABLE);
		}

		// Vertex Shader
		{
			vs = new VertexShader();
			vs->Create(ShaderPath + L"VertexColor.hlsl", "VS");
		}

		// Input Layout
		{
			il = new InputLayout();
			il->Create(VertexColor::descs, VertexColor::count, vs->GetBlob());
		}

		// Pixel Shader
		{
			ps = new PixelShader();
			ps->Create(ShaderPath + L"VertexColor.hlsl", "PS");
		}

		// World
		{
			wb = new WorldBuffer();
		}
	}

	for (int i = 0; i < 4; i++)
		verticesPosition[i] = vertices[i].position;

	edge	  = new RectEdge();
	collision = new BoundingBox({ 100, 100, 0 }, { 100, 100, 1 }, 0.0f, Color(1,0,0,1));
}

Rect::~Rect()
{
	SAFE_DELETE(collision);
	SAFE_DELETE(edge);

	SAFE_DELETE(wb);

	SAFE_DELETE(ps);
	SAFE_DELETE(il);
	SAFE_DELETE(vs);

	SAFE_DELETE(ib);
	SAFE_DELETE(vb);
}

void Rect::Update()
{
	UpdateWorld();
	collision->UpdateCollisionData();
}

void Rect::Render()
{
	vb->SetIA();
	ib->SetIA();
	il->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vs->SetShader();
	wb->SetVSBuffer(0);

	ps->SetShader();

	DC->DrawIndexed(ib->GetCount(), 0, 0);
}

void Rect::Move()
{
	if (Keyboard::Get()->Press('W'))
		position.y += 100 * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		position.y -= 100 * Time::Delta();

	if (Keyboard::Get()->Press('A'))
		position.x -= 100 * Time::Delta();
	else if (Keyboard::Get()->Press('D'))
		position.x += 100 * Time::Delta();
}

void Rect::GUI()
{
	ImGui::Begin("Rect", &bOpen);
	{
		ImGui::InputFloat3(MakeLable("Pos").c_str(), position, 2);
		ImGui::InputFloat3(MakeLable("Size").c_str(), size, 2);
		ImGui::SliderAngle(MakeLable("Rot").c_str(), &rotation, 0, 360.0f);
		if (ImGui::ColorPicker4("Color", color))
			UpdateColor();
	}
	ImGui::End();
}

void Rect::UpdateColor()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(vb->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (VertexColor& v : vertices)
			v.color = color;

		memcpy(subResource.pData, vertices.data(), vb->GetCount() * vb->GetStride());
	}
	DC->Unmap(vb->GetResource(), 0);
}

string Rect::MakeLable(string preFix)
{
	string label = preFix + "##" + to_string((uint)this);
	return label;
}

void Rect::UpdateWorld()
{
	S = DXMath::Scaling(size);
	R = DXMath::RotationInRadian(rotation);
	T = DXMath::Translation(position);

	world = S * R * T;
	wb->SetWorld(world);
}

void Rect::SetColor(Color color)
{
	this->color = color;
	UpdateColor();
}
