#include "Framework.h"
#include "Texture2D.h"

using namespace DirectX;
vector<TextureDesc> Textures::descs;

Texture2D::Texture2D(wstring filePath)
	:filePath(filePath)
{
	SRV::Get()->AddSRV(String::ToString(filePath),Textures::Load(this));
}

void Texture2D::ReadPixel(vector<Color>* pixels)
{
	ID3D11Texture2D* texture;
	srv->GetResource((ID3D11Resource**)&texture);

	ReadPixel(texture, pixels);
}

// 텍스처 픽셀을 읽어서 저장
void Texture2D::ReadPixel(ID3D11Texture2D* texture, vector<Color>* pixels)
{
	D3D11_TEXTURE2D_DESC srcDesc;	// 원본 텍스처의 정보
	texture->GetDesc(&srcDesc);		// 텍스처의 정보 복사

	D3D11_TEXTURE2D_DESC destDesc;	// 텍스처 정보의 복사본
	ZERO(destDesc);

	// 복사할 텍스처 desc 설정
	destDesc.Usage = D3D11_USAGE_STAGING;				// 사용 방식 설정
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;	// CPU 접근 권한 설정
	destDesc.Width = srcDesc.Width;						// 너비를 원본 텍스처와 같게 설정
	destDesc.Height = srcDesc.Height;					// 높이를 원본 텍스처와 같게 설정
	destDesc.MipLevels = 1;								// MipMap 레벨 1로 설정
	destDesc.ArraySize = 1;								// 배열 크기 1로 설정
	destDesc.Format = srcDesc.Format;					// 포맷을 원본과 같게 설정
	destDesc.SampleDesc = srcDesc.SampleDesc;			// 샘플링 방식을 원본과 같게 설정

	ID3D11Texture2D* destTex = nullptr;

	HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, &destTex);	// 복사할 텍스처를 생성 및 할당
	CHECK(hr);
	D3DX11LoadTextureFromTexture(DC, texture, nullptr, destTex);		// 원본 텍스처에서 복사할 텍스처로 데이터를 복사

	UINT* colors = new UINT[destDesc.Width * destDesc.Height];			// 픽셀 데이터를 저장할 배열

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(destTex, 0, D3D11_MAP_READ, 0, &subResource);
	{
		// subResource 구조체가 기리키는 데이터를 colors 배열에 복사
		memcpy(colors, subResource.pData, sizeof(UINT) * destDesc.Width * destDesc.Height);
	}
	DC->Unmap(destTex, 0);

	// 픽셀 개수 계산
	int pixelNum = destDesc.Width * destDesc.Height;

	// 색상 데이터의 RGBA 채널을 저장할 정수형 배열 생성
	UINT* alpha = new UINT[destDesc.Width * destDesc.Height];
	UINT* red   = new UINT[destDesc.Width * destDesc.Height];
	UINT* green = new UINT[destDesc.Width * destDesc.Height];
	UINT* blue  = new UINT[destDesc.Width * destDesc.Height];

	for (int i = 0; i < pixelNum; i++)
	{
		// colors 배열의 i 번째 원소에서 각각 채널의 비트를 추출하여 배열에 저장
		alpha[i] = (colors[i] & 0xFF000000);
		red[i]   = (colors[i] & 0x00FF0000);
		green[i] = (colors[i] & 0x0000FF00);
		blue[i]  = (colors[i] & 0x000000FF);

		// 하단에 기재
		red[i]  = colors[i] >> 16;
		blue[i] = colors[i] << 16;

		// 모든 배열의 i번째 원소를 or 연산하여 colors 배열에 다시 저장
		colors[i] = alpha[i] | red[i] | green[i] | blue[i];
	}

	// pixels 벡터에 colors 배열의 데이터를 할당
	pixels->assign(&colors[0], &colors[destDesc.Width * destDesc.Height - 1]);

	SAFE_DELETE_ARRAY(colors);
	SAFE_RELEASE(destTex);
}

void Textures::Delete()
{
	for (TextureDesc desc : descs)
		SAFE_RELEASE(desc.srv);
}

// texture 객체가 이미지 파일로부터 텍스처 데이터와 셰이더 리소스 뷰를 가지게 된다
ID3D11ShaderResourceView* Textures::Load(Texture2D* texture)
{
	HRESULT hr;
	TexMetadata metaData;

	// texture 객체의 filePath 속성에서 이미지 파일의 확장자를 얻어온다
	wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
		CHECK(hr);
	}
	else if (ext == L"dds")
	{
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, metaData);
		CHECK(hr);
	}
	else
	{
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, metaData);
		CHECK(hr);
	}

	// 메타 데이터에서 너비와 높이를 얻음
	UINT width = metaData.width;
	UINT height = metaData.height;

	TextureDesc desc;
	desc.filePath = texture->filePath;	// 구조체에 텍스처 경로 저장
	desc.width = width;					// 구조체에 텍스처 너비 저장
	desc.height = height;				// 구조체에 텍스처 높이 저장

	TextureDesc exist;
	bool bExist = false;				// 이미 존재하는지 검사하는 용도
	for (TextureDesc temp : descs)		// descs에 있는 모든 TextureDesc 구조체에 대해 반복
	{
		if (desc == temp)				// 현재 텍스처와 같은 텍스처가 존재하면
		{
			bExist = true;
			exist = temp;

			break;
		}
	}

	// 텍스처가 이미 존재한다면
	if (bExist == true)
	{
		texture->srv = exist.srv;	// texture객체 srv 속성에 exist 구조체의 srv 속성을 할당
		texture->metaData = metaData;
	}
	// 존재하지 않는다면
	else
	{
		// 이미지 파일로부터 텍스처 데이터를 읽고 쓰기 위해 사용
		ScratchImage image;

		// 확장자 검색 후 확장자에 맞는 함수를 호출하여 이미지 파일을 읽고 image 객체에 저장
		if (ext == L"tga")
		{
			hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
			CHECK(hr);
		}
		else if (ext == L"dds")
		{
			hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
			CHECK(hr);
		}
		else
		{
			hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
			CHECK(hr);
		}

		ID3D11ShaderResourceView* srv = nullptr;
		// image 객체로부터 셰이더 리소스 뷰를 생성하고 srv 포인터에 할당
		hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), metaData, &srv);
		CHECK(hr);

		SRV::Get()->AddSRV(String::ToString(texture->filePath), srv);

		desc.srv = srv;
		texture->srv = srv;
		texture->metaData = metaData;

		descs.push_back(desc);

		return srv;
	}
}

/*
DirectXTex
- 텍스처 처리 라이브러리
- DDS 파일을 읽기, 쓰기, 리사이징, 포맷 변환, 미핑맵 생성 등 여러 텍스처 콘텐츠 처리 작업을 수행

Metadata
- 너비, 높이, 깊이, 배열 크기, 미핑맵 수 및 형식과 같은 정보를 저장
*/