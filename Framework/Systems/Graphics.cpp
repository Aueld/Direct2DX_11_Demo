#include "Framework.h"
#include "Graphics.h"

// 스왑체인을 새로 조정하고, 렌더 타겟 뷰와 뷰포트를 업데이트
void Graphics::Resize(const UINT & width, const UINT & height)
{
	// 기존에 생성된 RTV 제거
	DeleteSurface();

	// 버퍼 크기 변경
	{
		HRESULT hr = swapChain->ResizeBuffers
		(
			0,						// 백 버퍼의 수 (0은 기존과 동일하게)
			width,					// 백 버퍼의 너비
			height,					// 백 버퍼의 높이
			DXGI_FORMAT_UNKNOWN,	// 백 버퍼의 포맷 (기존과 동일하게 유지)
			0						// 백 버퍼의 속성 (기존과 동일하게 유지)
		);
		assert(SUCCEEDED(hr));		// 리사이즈 실패한 경우 프로그램 종료
	}
	CreateRenderTargetView();		// 새로운 백 버퍼에 대한 RTV 생성
	SetViewport(width, height);		// 뷰포트를 새로운 크기에 맞게 설정
}

// 뷰포트 설정
void Graphics::SetViewport(const UINT & width, const UINT & height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width	  = (float)width;
	viewport.Height	  = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void Graphics::InitViewport()
{
	// OM 단계에서 사용할 뷰포트를 설정
	deviceContext->RSSetViewports(1, &viewport);
}

// 렌더링을 시작하기 전 필요한 초기화 작업을 수행
void Graphics::Begin()
{
	// RTV 설정
	deviceContext->OMSetRenderTargets	(1, &rtv, nullptr);
	// 뷰포트 설정
	deviceContext->RSSetViewports		(1, &viewport);
	// RTV를 지우고 clearColor로 색상을 채움
	deviceContext->ClearRenderTargetView(rtv, clearColor);
}

// 렌더링을 끝내고 화면에 결과를 출력
void Graphics::End()
{
	// 백 버퍼를 출력하고, 화면 갱신에 대해 대기할지 여부를 bVsync로 전달
	HRESULT hr = swapChain->Present(bVsync == true ?  1 : 0, 0);
	assert(SUCCEEDED(hr));
}

// Gui 구현
void Graphics::GUI()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ 0, 15 });
	ImGui::SetNextWindowSize(ImVec2(200, 30)); // 위젯 창 크기 설정
	ImGui::Begin
	(
		"Vstnc", 
		&bOpen,
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar
	);
	{
		ImGui::Checkbox("##Vsync", &bVsync);
	}
	ImGui::End();
}

// 스왑 체인을 생성
void Graphics::CreateSwapchain()
{
	// 이전에 할당된 메모리 해제
	{
		SAFE_RELEASE(device);
		SAFE_RELEASE(deviceContext);
		SAFE_RELEASE(swapChain);
	}

	// 구조체 초기화
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// 너비와 높이 설정
	desc.BufferDesc.Width  = 0;
	desc.BufferDesc.Height = 0;

	// 수직 동기화를 사용할 경우 프레임 레이트 설정
	{
		if (bVsync)
		{
			desc.BufferDesc.RefreshRate.Numerator   = adapterInfos[0]->outputInfo->numerator;
			desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
		}
		else
		{
			desc.BufferDesc.RefreshRate.Numerator   = 0;
			desc.BufferDesc.RefreshRate.Denominator = 1;
		}
	}

	// 버퍼의 쓰임새에 대해서 정의
	{
		// 버퍼의 색상 형식 설정
		desc.BufferDesc.Format			 = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 스케일링과 스캔라인 순서
		desc.BufferDesc.Scaling			 = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// 스왑 체인의 버퍼 개수, 용도, MSAA 품질 설정
		desc.BufferCount		= 1;
		desc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count	= 1;
		desc.SampleDesc.Quality = 0;

		// 출력 창 핸들, 창 모드, 스왑 체인 효과 설정
		desc.OutputWindow = handle;
		desc.Windowed	  = true;
		desc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	}

	// 사용 가능한 D3D 기능 수준을 나타내는 D3D_FEATURE_LEVEL 배열 생성
	vector<D3D_FEATURE_LEVEL> featureLevel
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	// 가장 VRAM이 큰 어댑터를 선택하여 selectedAdapterIndex에 할당
	UINT maxVRam = 0;
	for (UINT i = 0; i < adapterInfos.size(); i++)
	{
		if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVRam)
		{
			selectedAdapterIndex = i;
			maxVRam = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
		}
	}

	D3D_FEATURE_LEVEL fl;
	// 디바이스와 스왑 체인 생성
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		adapterInfos[selectedAdapterIndex]->adapter,	// 최대 VRAM 용량을 가진 그래픽 장치 선택
		D3D_DRIVER_TYPE_UNKNOWN,						// 드라이버 타입 (자동)
		nullptr,										// 소프트웨어 렌더러 사용 안함
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,				// 렌더링 시 필요한 플래그 설정
		featureLevel.data(),							// 사용할 Direct3D 기능 레벨
		featureLevel.size(),							// 사용할 Direct3D 기능 레벨의 개수
		D3D11_SDK_VERSION,								// DirectX SDK 버전
		&desc,											// 스왑 체인을 생성할 때 사용할 구조체
		&swapChain,										// 생성된 스왑 체인 객체 반환
		&device,										// 생성된 디바이스 객체 반환
		&fl,											// 사용하지 않음
		&deviceContext									// 생성된 디바이스 컨텍스트 객체를 반환
	);
	assert(SUCCEEDED(hr));

	// 사이즈 재설정
	Resize(WinMaxWidth, WinMaxHeight);

	// cmd 출력
	{
		for (int i = 0; i < adapterInfos.size(); i++)
		{
			gpuName = adapterInfos[i]->adapterDesc.Description;
			wcout << "GPU Name : " << adapterInfos[i]->adapterDesc.Description << endl;
			cout << "VRAM : " << adapterInfos[i]->adapterDesc.DedicatedVideoMemory << endl;
			cout << endl;
		}

		wcout << "Selected GPU Name : "
			<< adapterInfos[selectedAdapterIndex]->
			adapterDesc.Description << endl;
	}

}

// 스왑체인에서 백 버퍼를 가져와 RTV 생성
void Graphics::CreateRenderTargetView()
{
	// 백버퍼 가져오기
	ID3D11Texture2D* backbuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backbuffer
	);
	assert(SUCCEEDED(hr));

	// 백 버퍼로 RTV 생성
	hr = device->CreateRenderTargetView
	(
		backbuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	// 백 버퍼 해제
	SAFE_RELEASE(backbuffer);
}

// RTV 해제
void Graphics::DeleteSurface()
{
	SAFE_RELEASE(rtv);
}

// 어댑터를 검색하고 스왑체인 생성
Graphics::Graphics()
{
	EnumerateAdapters();
	CreateSwapchain();
}

// 생성한 자원을 해제
Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

// 그래픽 어댑터를 찾아 정보를 저장
void Graphics::EnumerateAdapters()
{
	// DXGI 객체를 생성하는 함수를 구현한 인터페이스
	IDXGIFactory1* factory;
	// 팩토리 생성
	if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
		// 실패시 종료
		return;

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter = nullptr;
		// 어댑터를 나열
		HRESULT hr = factory->EnumAdapters1(index, &adapter);

		// 더 이상 어댑터가 없으면 종료
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		// 나열에 실패하면 프로그램 종료
		assert(SUCCEEDED(hr));

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		// 어댑터의 인덱스 저장
		adapterInfo->adapterOrdinal = index;
		// 어댑터의 정보 저장
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		// 어댑터 정보 저장
		adapterInfo->adapter = adapter;

		// 어댑터의 출력 장치 나열
		EnumerateAdapterOutput(adapterInfo);
		// 어댑터 정보 저장
		adapterInfos.push_back(adapterInfo);

		// 다음 어댑터로 이동
		index++;
	}
	// 팩토리 해제
	SAFE_RELEASE(factory);
}

// 어댑터에서 지원하는 출력 모드를 가져오는 역할, 출력 빈도를 저장
bool Graphics::EnumerateAdapterOutput(D3DEnumAdapterInfo * adapterInfo)
{
	IDXGIOutput* output = nullptr;
	// 어댑터의 출력 장치 나열
	HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);

	// 출력 장치가 없으면 함수 종료
	if (DXGI_ERROR_NOT_FOUND == hr)
		return false;
	// 나열에 실패하면 프로그램 종료
	assert(SUCCEEDED(hr));

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();
	ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));

	
	output->GetDesc(&outputInfo->outputDesc);	// 출력 장치의 정보 저장
	outputInfo->output = output;				// 출력 장치 객체 저장

	UINT numModes				 = 0;
	DXGI_MODE_DESC* displayModes = nullptr; 
	DXGI_FORMAT format			 = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 출력 장치의 디스플레이 모드 개수 확인
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	assert(SUCCEEDED(hr));

	
	displayModes = new DXGI_MODE_DESC[numModes];
	// 출력 장치의 디스플레이 모드 정보 저장
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < numModes; i++)
	{
		bool bCheck = true;
		bCheck &= displayModes[i].Width == WinMaxWidth;		// 디스플레이 모드의 너비가 최대 너비와 같은지 확인
		bCheck &= displayModes[i].Height == WinMaxHeight;	// 디스플레이 모드의 높이가 최대 높이와 같은지 확인

		if (bCheck == true)
		{
			// 디스플레이 모드의 리프레시 비율 저장
			outputInfo->numerator  = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}

	// 출력 장치 정보 저장
	adapterInfo->outputInfo = outputInfo;
	SAFE_DELETE_ARRAY(displayModes);
	return true;
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
	SAFE_RELEASE(adapter);
	SAFE_DELETE(outputInfo);
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
	SAFE_RELEASE(output);
}

/*
Graphic Pipeline
- 3D 그래픽의 출력 결과를 생성하기 위해 여러 단계를 거처 처리하는 과정
- CPU와 GPU의 협업으로 이루어진다

01. IA (입력 어셈블러, Input Assembler)
- 정점 버퍼와 인덱스 버퍼를 사용하여 기본 도형(프리미티브)을 생성

02. VS (정점 쉐이더, Vertex Shader)
- IA에서 생성된 프리미티브에 대한 버텍스 데이터를 처리
- 각각의 버텍스는 정점 위치, 텍스처 좌표, 노멀 벡터 등의 정보를 가지고 있다
- 이러한 정보를 바탕으로 쉐이더는 각 버텍스의 최종 위치를 계산

03. HS (헐 쉐이더, Hull Shader)
- 공간 분할 단계 중 하나로, 모델의 한 표면을 효율적으로 많은 삼각형으로 나눈다
- 각 입력 패치 (사각형, 삼각형 또는 선)에 해당하는 기하 도형 패치 (및 패치 상수)를 생성
-  헐 셰이더는 패치당 한 번 호출되어, 낮은 차수의 표면을 정의하는 입력 제어점을 패치를 구성하는 제어점으로 변환

04. TS (분할기, Tessellator)
- 테셀레이션 단계는 하드웨어에서 테셀레이션을 구현
- 그래픽 파이프라인은 낮은 디테일 (낮은 다각형 수) 모델을 평가하고 더 높은 디테일로 렌더링 할 수 있다
- 더 높은 차수의 표면을 Direct3D 그래픽 파이프라인 내에서 자세한 렌더링을 위한 많은 삼각형으로 변환하는 데 함께 작동

05. DS (도메인 쉐이더, Domain Shader)
- 출력 패치의 세분화된 지점의 꼭지점 위치를 출력
- 테셀레이션에서 생성된 패치 정보를 받아서, 모델을 구성하는 도형을 만듦

06. GS (지오메트리 쉐이더, Geometry Shader)
- 도메인 셰이더에서 생성된 도형 정보를 가지고 추가적인 변환을 수행하거나, 기하학적 모양을 만들어냅니다.

07. SO (스트림 출력, Stream Output)
- 정점 데이터를 캡처하고 저장합니다.

08. RS (래스터라이저, Rasterizer)
- 3D 공간에서 정의된 3D 모델을 2D 화면으로 변환하는 과정
- 각 3D 폴리곤을 2D 픽셀로 변환하고, 이 픽셀에 대한 정보(색상, 깊이 등)을 계산하여 RTV에 출력

09. PS (픽셀 쉐이더, Pixel Shader)
- 화면에 출력되는 각 픽셀에 대한 정보(색상, 깊이 등)를 계산
- 모델이 화면에서 차지하는 픽셀의 개수만큼 픽셀 쉐이더가 실행
- 색상, 그림자, 광원 등을 계산하여 화면에 출력

10. OM (출력 병합기, Output Merger)
- 렌더링된 픽셀을 프레임 버퍼에 병합합니다.
*/