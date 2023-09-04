#include "Framework.h"
#include "IShader.h"

void IShader::CompileShader(wstring path, string entryName, string profile, ID3DBlob** blob)
{
	ID3DBlob* error = nullptr;			// �����Ϸ� ���� �޼����� ������ Blob �����͸� nullptr�� �ʱ�ȭ
	HRESULT  hr = D3DCompileFromFile	// ���̴� ������ �������ϰ�, ����� Blob �����ͷ� ���޹���
	(
		path.c_str(),					// �������� ���̴� ���� ���
		nullptr,						// ���̴� ������ ����� ��Ÿ���� ������
		nullptr,						// Include �������̽��� ������ Ŭ������ ����� ���̴� ���Ͽ��� �ٸ� ������ include�� �� ����� �� �ִ� ���ý�Ʈ
		entryName.c_str(),				// �������� ���̴� ���Ͽ��� ���� ���������� ����� �Լ� �̸�
		profile.c_str(),				// �������� ���̴� ���Ͽ��� ����� ������ �̸�
		D3DCOMPILE_ENABLE_STRICTNESS,	// ������ ������ �ɼ� / ��� �޼����� ���� 1�� ����
		0,								// ����� ������ ���Ե� ������ ����� �����ϱ� ���� �ɼ�
		blob,							// ������ ����� ������ blob ������ �ּҰ�
		&error							// �����Ϸ� ���� �޼����� ������ blob ������ �ּҰ�
	);
	CheckShaderError(hr, error);		// �����Ϸ� ������ �߻��ϸ� �����޼����� ���
	SAFE_RELEASE(error);				// blob ������ ����
}

// ���̴��� ������ ������ ���� �ڽ��� ����ϴ� �Լ�
void IShader::CheckShaderError(HRESULT hr, ID3DBlob* error)
{
	// HRESULT�� ������ ���
	if (FAILED(hr))
	{
		// error�� ������ ���
		if (error)
		{
			// error �޼����� ���ڿ��� �����ͼ� �޼��� �ڽ��� error�� ���
			string str = (const char*)error->GetBufferPointer();
			MessageBoxA(handle, str.c_str(), "Shader Error", MB_OK);
		}
		// ���α׷� �ߴ�
		CHECK(false);
	}
}

/*
ID3DBlob
- �޸𸮿� �Ҵ�� ������ ���
- ��ǥ������ ���̴� �ڵ峪 ���� ������ ��
- �޸𸮿� �Ҵ�� ������ ����� �����ϰ� ������ ������ �� ����
*/