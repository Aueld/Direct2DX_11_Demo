struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0)
{
    matrix _world;
};

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

cbuffer BlurBuffer : register(b1)
{
    float2 _imageSize;
    int _blurCount;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    
    output.uv = input.uv;
    return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = _sourceTex.Sample(_samp, (float2) input.uv);
    
    // 주변 픽셀들
    float2 arr[8] =
    {
        float2(-1, +1), float2(+0, +1), float2(+1, +1),
        float2(-1, +0), /* 기준정점 */   float2(+1, +0),
        float2(-1, -1), float2(+0, -1), float2(+1, -1)
    };
    
    // 블러 카운트만큼 반복
    for (int blur = 1; blur < _blurCount; blur++)
    {
        // 주변 정점들에 대해 반복
        for (int i = 0; i < 8; i++)
        {
            // 입력 텍스처에서 샘플링할 주변 픽셀들의 상대적인 위치 계산
            float x = arr[i].x * (float) blur / _imageSize.x;
            float y = arr[i].y * (float) blur / _imageSize.y;
            
            // 새로운 uv 좌표 계산
            float2 uv = input.uv + float2(x, y);
            // 입력 텍스처에 색상을 추가로 샘플링하여 누적
            color += _sourceTex.Sample(_samp, uv);

        }
    }
    // 최종 색상을 게산
    // blurCount - 1번 반복하면서 각 반복마다 8개의 오프셋에 대해 색상을 샘플링 하였으므로,
    // 최종적으로 (_blurCount - 1) * 8 번의 색상이 누적된다
    // 여기에 처음에 입력 텍스처에서 샘플링한 원래 픽셀의 색상도 포함되므로 전체적으로 (_blurCount - 1) * 8 + 1번의 색상이 누적
    // 최종적인 평균 색상값을 계산하기 위해서는 나눠줘야 한다
    
    // 블러 반복 횟수
    int blurIterations = _blurCount - 1;
    // 오프셋 개수
    int offsetCount = 8;
    // 전체 샘플링 개수
    int totalSamples = blurIterations * offsetCount + 1;
    
    return color /= totalSamples;
}