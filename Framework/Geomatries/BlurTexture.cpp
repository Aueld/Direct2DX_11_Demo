#include "Framework.h"
#include "BlurTexture.h"

BlurTexture::BlurTexture(Vector3 position, Vector3 size, float rotation, wstring path)
	:TextureRect(position, size, rotation, path)
{
	buffer = new BlurBuffer();
	buffer->SetTextureSize({ size.x, size.y });
	SetShader(ShaderPath + L"TextureBlur.hlsl");
}

BlurTexture::~BlurTexture()
{
	SAFE_DELETE(buffer);
}

void BlurTexture::Update()
{
	__super::Update();
}

void BlurTexture::Render()
{
	buffer->SetPSBuffer(1);
	__super::Render();
}

void BlurTexture::GUI()
{
	ImGui::Begin("BlurCount");
	{
		ImGui::SliderInt("BlurCount", buffer->GetBlurCountPtr(), 1, 20);
	}
	ImGui::End();
}