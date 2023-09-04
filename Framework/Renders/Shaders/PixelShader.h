// 렌더 타겟에 그려진 픽셀들의 최종 색상을 계산하는 역할

#pragma once
#include "IShader.h"

class PixelShader : public IShader
{
public:
	~PixelShader();

	virtual void Create(const wstring path, const string entryName) override;
	virtual void Clear() override;
	virtual void SetShader() override;

	ID3DBlob* GetBlob() { return blob; }
	ID3D11PixelShader* GetResource() { return shader; }

private:
	ID3D11PixelShader* shader = nullptr;
	ID3DBlob* blob = nullptr;
};