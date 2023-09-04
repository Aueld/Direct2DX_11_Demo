// 버택스 정보를 저장하기 위한 구조체

#pragma once

// 색상
struct VertexColor
{
	VertexColor() 
		: position(0, 0, 0), color(0, 0, 0, 0) {}

	VertexColor(Vector3 position, Color color) 
		: position(position), color(color) {}
	
	// 정점 위치
	Vector3 position;
	// 정점 색상
	Color color;

	// 정점 데이터 구조를 서술
	static D3D11_INPUT_ELEMENT_DESC descs[];
	// 원소 개수
	static const uint count = 2;
};

// 텍스처
struct VertexTexture
{
	VertexTexture()
		: position(0, 0, 0), uv(0, 0) {}

	VertexTexture(Vector3 position, Vector2 uv)
		: position(position), uv(uv) {}

	Vector3 position;
	// 텍스트 좌표
	Vector2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

// 텍스처 + 색상
struct VertexTextureColor
{
	VertexTextureColor()
		: position(Values::ZeroVec3), uv(Values::ZeroVec2), color(0, 0, 0, 0) {}

	VertexTextureColor(Vector3 position, Vector2 uv, Color color)
		: position(position), uv(uv), color(color) {}

	Vector3 position;
	Vector2 uv;
	Color color;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 3;
};

// 타일
struct VertexTile
{
	VertexTile()
		: position(Values::ZeroVec3), uv(Values::ZeroVec2), color(0, 0, 0, 0) {}

	VertexTile(Vector3 position, Vector2 uv, Vector2 uv2, Color color, uint index, bool bWalkable = true)
		: position(position), uv(uv), uv2(uv2), color(color), index(index), bWalkable(bWalkable) {}

	Vector3 position;
	Vector2 uv;
	Vector2 uv2;
	Color color;
	uint index;
	bool bWalkable;
	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 6;
};

/*
	D3D11_INPUT_ELEMENT_DESC

	SemanticName		 : 정점 데이터의 의미를 나타내는 문구
	SemanticIndex		 : 위 이름이 중복되는 경우 구분하기 위한 인덱스 값
	Format				 : 데이터 형식을 나타냄
	InputSlot			 : 입력 슬롯의 인덱스
	AlignedByteOffset	 : 정점 데이터의 바이트 단위 오프셋 (0 = 자동계산)
	InputSlotClass		 : 사용되는 데이터 형식을 지정
	InstanceDataStepRate : 인스턴스마다 데이터를 읽어들이는데 사용할 스텝 레이트 값
*/