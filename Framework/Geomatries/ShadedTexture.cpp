#include "Framework.h"
#include "ShadedTexture.h"

ShadedTexture::ShadedTexture(Vector3 position, Vector3 size, float rotation, wstring path)
	:TextureRect(position, size, rotation, path)
{
	sb = new ShadedBuffer();
}

ShadedTexture::~ShadedTexture()
{
	SAFE_DELETE(sb);
}

void ShadedTexture::Render()
{
	sb->SetPSBuffer(0);
	TextureRect::Render();
}

void ShadedTexture::GUI()
{
	ImGui::Begin("Shaded");
	{
		ImGui::RadioButton("Normal", sb->GetSelectionPtr(), 1);
		ImGui::RadioButton("AM", sb->GetSelectionPtr(), 2);
		ImGui::RadioButton("Sepia", sb->GetSelectionPtr(), 3);
		ImGui::RadioButton("Negative", sb->GetSelectionPtr(), 4);
		ImGui::RadioButton("Posterize", sb->GetSelectionPtr(), 5);
	}
	ImGui::End();
}
