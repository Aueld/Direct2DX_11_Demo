// VertexInput 구조체 정의
struct VertexInput
{
    float4 position : POSITION0;    // 정점 위치    "position"이라는 변수가 "POSITION0" 스트림의 위치에 대한 정보를 저장한다
    float2 uv : TEXCOORD0;          // uv 좌표    
};

// PixelInput 구조체 정의
struct PixelInput
{
    float4 position : SV_POSITION0; // 픽셀 위치    픽셀의 위치를 나타내는 시스템 값을 의미
    float2 uv : TEXCOORD0;          // 픽셀 색상
};

// 상수 버퍼 레지스터 b0에 할당된 월드 행렬을 저장하는 상수 버퍼
cbuffer WorldBuffer : register(b0) // 0 ~ 127
{
    matrix _world;
};

// 상수 버퍼 레지스터 b1에 할당된 뷰 행렬과, 프로젝션 행렬을 저장하는 상수 버퍼
cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

// 정점 셰이더 함수
// 입력으로 VertexInput을 받고 PixelInput 구조체를 반환
PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _world);          // 정점 위치에 월드 행렬을 곱함
    output.position = mul(output.position, _view);          // 결과에 뷰 행렬을 곱함
    output.position = mul(output.position, _projection);    // 결과에 프로젝션 매트릭스 곱함
    
    output.uv = input.uv;
    return output;
}

Texture2D _sourceTex : register(t0);    // 텍스처 데이터를 저장, 텍스처 슬롯 0번에 바인딩
SamplerState _samp : register(s0);      // 샘플링하는 방법을 지정, 샘플러 슬롯 0번에 바인딩

// 픽셀 셰이더 함수
// 입력으로 PixelInput 구조체를 받고, float4 형태의 픽셀 색상을 반환
float4 PS(PixelInput input) : SV_Target
{
    float4 color = _sourceTex.Sample(_samp, (float2) input.uv);
    
    return color;
}

/*
Semantic : 세멘틱
- HLSL 셰이더에서 데이터의 의미를 지정하는 데 사용
- 셰이더가 입력 데이터와 출력 데이터를 올바르게 해석하고 처리할 수 있도록 해줌
- 변수 이름 뒤에 ':' 기호와 함께 지정
- 시스템 값 세멘틱은 'SV_' 접두사로 시작하며 Direct3D에서 정의된 특별한 의미를 가짐
- 시스템 값 세멘틱은 셰이더 스테이지 간에 데이터를 전달하는데 사용

System-Value Semantic : 시스템 값 세멘틱
1. SV_Position  : 정점 셰이더 출력에서 픽셀 셰이더 입력으로 클립 공간 위치 정보를 전달하는데 사용
2. SV_Target    : 픽셀 셰이더의 출력에서 렌더 타겟으로 색상 정보를 전달하는 데 사용
3. SV_Depth     : 픽셀 셰이더의 출력에서 깊이 버퍼로 깊이 정보를 전달하는 데 사용
(자세한 내용은 Direct3D 문서 참조)

slot
- GPU에서 사용하는 상수 버퍼, 텍스처, 샘플러 등의 자원들을 식별하는 인덱스
- 각 슬롯을 고유한 번호를 가지며, 해당 자원의 유형과 역할에 따라 다르게 할당됨
- register라는 키워드를 사용하여 지정
- 각 자원이 어떤 슬롯에 할당될 것인지 명시적으로 지정할 수 있음
- 주요 슬롯
    1) 상수 버퍼 슬롯
    - 상수 데이터를 저장하는데 사용되며, VS와 PS에서 공유될 수 있다
    - 상수 버퍼 슬롯은 register(b#)을 사용하여 지정
    
    2) 텍스처 슬롯
    - 이미지 데이터를 저장하는데 사용
    - 텍스처 버퍼 슬롯은 register(t#)을 사용하여 지정

    3) 샘플러 슬롯
    - 텍스처를 샘플링하는 데 사용
    - 샘플러 슬롯은 register(s#)을 사용하여 지정

cbuffer = Constant Buffer : 상수 버퍼
- 셰이더에서 사용하는 전역 변수를 저장하는데 사용
- 각 상수 버퍼 레지스터에는 한 개의 상수 버퍼만 할당할 수 있음
- 상수 버퍼 내부에는 여러 개의 변수를 선언할 수 있다

Sampler : 샘플러
- 텍스처에서 픽셀 값을 가져오는 방법을 정의하는 객체
- 텍스처 샘플링은 텍스처 이미지에 대해 텍셀 값을 계산하는 작업
  (텍셀 : 텍스처의 픽셀 값)
*/