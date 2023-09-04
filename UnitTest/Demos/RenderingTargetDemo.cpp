#include "stdafx.h"
#include "RenderingTargetDemo.h"

#include "Geomatries/Rect.h"
#include "Geomatries/TextureRect.h"
#include "Utilities/RenderTexture.h"

void RenderingTargetDemo::Init()
{
	rtt = new RenderTexture;

	Vector3 size = Vector3(100, 100, 1);
	Vector3 trPos = Vector3(WinMaxWidth - (WinMaxWidth / 4), WinMaxHeight - (WinMaxHeight / 4), 0);
	Vector3 trSiz = Vector3(WinMaxWidth / 2, WinMaxHeight / 2, 0);
	float rot = 0;
	
	r1 = new Rect(Vector3(200, 200, 0), size, rot);
	r2 = new Rect(Vector3(500, 400, 0), size, rot);
	r3 = new Rect(Vector3(700, 100, 0), size, rot);

	tr1 = new TextureRect(Vector3(100, 100, 0), Vector3(500, 500, 1), 0.f, TexturePath + L"error.jpg");
	tr2 = new TextureRect(trPos, trSiz, rot);
	
	tr2->SetShader(ShaderPath + L"VertexTextureOutLine.hlsl");
}

void RenderingTargetDemo::Destroy()
{
	SAFE_DELETE(tr2);
	SAFE_DELETE(tr1);
	SAFE_DELETE(r3);
	SAFE_DELETE(r2);
	SAFE_DELETE(r1);
	SAFE_DELETE(rtt);
}

void RenderingTargetDemo::Update()
{
	r1->Update();
	r2->Update();
	r3->Update();

	tr1->Update();
	tr2->Update();
}

void RenderingTargetDemo::Render()
{
	r1->Render();
	r2->Render();
	r3->Render();

	tr1->Render();
	tr2->SetSRV(rtt->GetSRV());
	tr2->Render();
}

// PostRender 자체를 Windows.cpp 에서 Render() 후에 처리 -> 위에 그려짐
void RenderingTargetDemo::PostRender()
{
	Graphics::Get()->InitViewport();
	rtt->RenderToTexture();
	{
		r1->Render();
		r2->Render();
		r3->Render();

		tr1->Render();
	}
}

void RenderingTargetDemo::GUI()
{

}
