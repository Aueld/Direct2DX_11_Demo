// 가상 함수를 사용하여 DirectX에서 셰이더를 만들고 사용할 때 사용하는 인터페이스

#pragma once

class IShader
{
public:
	// 생성
	virtual void Create(const wstring path, const string entryName) = 0;
	// 객체 초기화
	virtual void Clear()	 = 0;
	// 셰이더 세팅
	virtual void SetShader() = 0;

protected:
	void CompileShader(wstring path, string entryName, string profile, ID3DBlob** blob);

	wstring path = L"";
	string entryName = "";

private:
	void CheckShaderError(HRESULT hr, ID3DBlob* error);
};