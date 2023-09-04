#include "stdafx.h"
#include "ShadedDemo.h"

#include "Geomatries/TextureRect.h"
#include "Geomatries/ShadedTexture.h"

void ShadedDemo::Init()
{
	st = new ShadedTexture(Vector3(620, 360, 0), Vector3(500, 500, 1), 0.0f, TexturePath + L"error.jpg");
	st->SetShader(ShaderPath + L"TextureFitter.hlsl");
}

void ShadedDemo::Destroy()
{
	SAFE_DELETE(st);
}

void ShadedDemo::Update()
{
	st->Update();
}

void ShadedDemo::Render()
{
	st->Render();
}

void ShadedDemo::PostRender()
{
}

void ShadedDemo::GUI()
{
	st->GUI();
}
