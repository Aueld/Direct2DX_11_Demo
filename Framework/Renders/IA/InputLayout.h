// 정점 데이터의 레이아웃을 정의하고 설정하는 역할

#pragma once

class InputLayout
{
public:
	~InputLayout();

	void Create(D3D11_INPUT_ELEMENT_DESC* descs, uint count, ID3DBlob* blob);

	void SetIA();

private:
	ID3D11InputLayout* inputLayout = nullptr;
};