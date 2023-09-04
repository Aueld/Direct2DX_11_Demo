// 정점들의 위치, 색상, UV좌표 등의 속성을 계산

#pragma once
#include "IShader.h"

class VertexShader : public IShader
{
public:

	~VertexShader();

	virtual void Create(const wstring path, const string entryName) override;
	virtual void Clear() override;
	virtual void SetShader() override;

	ID3DBlob* GetBlob() { return blob; }
	ID3D11VertexShader* GetResource() { return shader; }

private:
	ID3D11VertexShader* shader = nullptr;
	ID3DBlob* blob = nullptr;
};