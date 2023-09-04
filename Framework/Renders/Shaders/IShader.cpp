#include "Framework.h"
#include "IShader.h"

void IShader::CompileShader(wstring path, string entryName, string profile, ID3DBlob** blob)
{
	ID3DBlob* error = nullptr;			// 컴파일러 오류 메세지를 저장할 Blob 포인터를 nullptr로 초기화
	HRESULT  hr = D3DCompileFromFile	// 쉐이더 파일을 컴파일하고, 결과를 Blob 포인터로 전달받음
	(
		path.c_str(),					// 컴파일할 쉐이더 파일 경로
		nullptr,						// 쉐이더 파일의 헤더를 나타내는 포인터
		nullptr,						// Include 인터페이스를 구현한 클래스를 사용해 쉐이더 파일에서 다른 파일을 include할 때 사용할 수 있는 컨택스트
		entryName.c_str(),				// 컴파일할 쉐이더 파일에서 최초 진입점으로 사용할 함수 이름
		profile.c_str(),				// 컴파일할 쉐이더 파일에서 사용할 프로필 이름
		D3DCOMPILE_ENABLE_STRICTNESS,	// 엄격한 컴파일 옵션 / 경고 메세지를 레벨 1로 설정
		0,								// 디버그 정보가 포함된 컴파일 결과를 생성하기 위한 옵션
		blob,							// 컴파일 결과를 저장할 blob 포인터 주소값
		&error							// 컴파일러 오류 메세지를 저장할 blob 포인터 주소값
	);
	CheckShaderError(hr, error);		// 컴파일러 오류가 발생하면 오류메세지를 출력
	SAFE_RELEASE(error);				// blob 포인터 해제
}

// 쉐이더에 에러가 있으면 에러 박스를 출력하는 함수
void IShader::CheckShaderError(HRESULT hr, ID3DBlob* error)
{
	// HRESULT가 실패할 경우
	if (FAILED(hr))
	{
		// error가 존재할 경우
		if (error)
		{
			// error 메세지를 문자열로 가져와서 메세지 박스로 error를 출력
			string str = (const char*)error->GetBufferPointer();
			MessageBoxA(handle, str.c_str(), "Shader Error", MB_OK);
		}
		// 프로그램 중단
		CHECK(false);
	}
}

/*
ID3DBlob
- 메모리에 할당된 데이터 덩어리
- 대표적으로 쉐이더 코드나 버퍼 데이터 등
- 메모리에 할당된 데이터 덩어리를 조작하고 정보를 추출할 수 있음
*/