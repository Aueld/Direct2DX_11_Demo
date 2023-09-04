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
    
    // �ֺ� �ȼ���
    float2 arr[8] =
    {
        float2(-1, +1), float2(+0, +1), float2(+1, +1),
        float2(-1, +0), /* �������� */   float2(+1, +0),
        float2(-1, -1), float2(+0, -1), float2(+1, -1)
    };
    
    // �� ī��Ʈ��ŭ �ݺ�
    for (int blur = 1; blur < _blurCount; blur++)
    {
        // �ֺ� �����鿡 ���� �ݺ�
        for (int i = 0; i < 8; i++)
        {
            // �Է� �ؽ�ó���� ���ø��� �ֺ� �ȼ����� ������� ��ġ ���
            float x = arr[i].x * (float) blur / _imageSize.x;
            float y = arr[i].y * (float) blur / _imageSize.y;
            
            // ���ο� uv ��ǥ ���
            float2 uv = input.uv + float2(x, y);
            // �Է� �ؽ�ó�� ������ �߰��� ���ø��Ͽ� ����
            color += _sourceTex.Sample(_samp, uv);

        }
    }
    // ���� ������ �Ի�
    // blurCount - 1�� �ݺ��ϸ鼭 �� �ݺ����� 8���� �����¿� ���� ������ ���ø� �Ͽ����Ƿ�,
    // ���������� (_blurCount - 1) * 8 ���� ������ �����ȴ�
    // ���⿡ ó���� �Է� �ؽ�ó���� ���ø��� ���� �ȼ��� ���� ���ԵǹǷ� ��ü������ (_blurCount - 1) * 8 + 1���� ������ ����
    // �������� ��� ������ ����ϱ� ���ؼ��� ������� �Ѵ�
    
    // �� �ݺ� Ƚ��
    int blurIterations = _blurCount - 1;
    // ������ ����
    int offsetCount = 8;
    // ��ü ���ø� ����
    int totalSamples = blurIterations * offsetCount + 1;
    
    return color /= totalSamples;
}