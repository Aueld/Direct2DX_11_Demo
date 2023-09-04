// VertexInput ����ü ����
struct VertexInput
{
    float4 position : POSITION0; // ���� ��ġ    "position"�̶�� ������ "POSITION0" ��Ʈ���� ��ġ�� ���� ������ �����Ѵ�
    float2 uv       : TEXCOORD0; // uv ��ǥ    
};

// PixelInput ����ü ����
struct PixelInput
{
    float4 position : SV_POSITION0; // �ȼ� ��ġ    �ȼ��� ��ġ�� ��Ÿ���� �ý��� ���� �ǹ�
    float2 uv       : TEXCOORD0;    // �ȼ� ����
};

// ��� ���� �������� b0�� �Ҵ�� ���� ����� �����ϴ� ��� ����
cbuffer WorldBuffer : register(b0)
{
    matrix _world;
};

// ��� ���� �������� b1�� �Ҵ�� �� ��İ�, �������� ����� �����ϴ� ��� ����
cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
};

cbuffer ShadedBuffer : register(b0)
{
    int Selection;
};

// ���� ���̴� �Լ�
// �Է����� VertexInput�� �ް� PixelInput ����ü�� ��ȯ
PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _world);          // ���� ��ġ�� ���� ����� ����
    output.position = mul(output.position, _view);          // ����� �� ����� ����
    output.position = mul(output.position, _projection);    // ����� �������� ��Ʈ���� ����
    
    output.uv = input.uv;
    return output;
}

Texture2D _sourceTex : register(t0);    // �ؽ�ó �����͸� ����, �ؽ�ó ���� 0���� ���ε�
SamplerState _samp   : register(s0);    // ���ø��ϴ� ����� ����, ���÷� ���� 0���� ���ε�

// �ȼ� ���̴� �Լ�
// �Է����� PixelInput ����ü�� �ް�, float4 ������ �ȼ� ������ ��ȯ
float4 PS(PixelInput input) : SV_Target
{
    float4 color = _sourceTex.Sample(_samp, (float2) input.uv);
    float4 resultColor = 0;
    
    if (Selection == 1)
    {
        return color;
    }
    else if (Selection == 2)
    {
        // Gray
        float3 temp = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
        resultColor = float4(temp, color.a);
    }
    else if (Selection == 3)
    {
        // Sepia
        float3 sepia = float3(0.393f, 0.769f, 0.189f);
        float3 temp = dot(color.rgb, sepia);
        temp.r += 0.2;
        temp.g += 0.1;
        resultColor = float4(temp, color.a);
    }
    else if (Selection == 4)
    {
        // Negative
        float3 negative = 1 - abs(color.rgb);
        resultColor = float4(negative, color.a);
    }
    else if (Selection == 5)
    {
        // Posterize
        float3 value = float3(4.0f, 4.0f, 4.0f);    // ���� ���������� ¸�� ������ ����
        float3 posterize = round(color.rgb * value) / value;
        resultColor = float4(posterize.rgb, color.a);
    }
    else
    {
        resultColor = color;
    }
    
    return resultColor;
}

/*
Semantic : ����ƽ
- HLSL ���̴����� �������� �ǹ̸� �����ϴ� �� ���
- ���̴��� �Է� �����Ϳ� ��� �����͸� �ùٸ��� �ؼ��ϰ� ó���� �� �ֵ��� ����
- ���� �̸� �ڿ� ':' ��ȣ�� �Բ� ����
- �ý��� �� ����ƽ�� 'SV_' ���λ�� �����ϸ� Direct3D���� ���ǵ� Ư���� �ǹ̸� ����
- �ý��� �� ����ƽ�� ���̴� �������� ���� �����͸� �����ϴµ� ���

System-Value Semantic : �ý��� �� ����ƽ
1. SV_Position  : ���� ���̴� ��¿��� �ȼ� ���̴� �Է����� Ŭ�� ���� ��ġ ������ �����ϴµ� ���
2. SV_Target    : �ȼ� ���̴��� ��¿��� ���� Ÿ������ ���� ������ �����ϴ� �� ���
3. SV_Depth     : �ȼ� ���̴��� ��¿��� ���� ���۷� ���� ������ �����ϴ� �� ���
(�ڼ��� ������ Direct3D ���� ����)

slot
- GPU���� ����ϴ� ��� ����, �ؽ�ó, ���÷� ���� �ڿ����� �ĺ��ϴ� �ε���
- �� ������ ������ ��ȣ�� ������, �ش� �ڿ��� ������ ���ҿ� ���� �ٸ��� �Ҵ��
- register��� Ű���带 ����Ͽ� ����
- �� �ڿ��� � ���Կ� �Ҵ�� ������ ��������� ������ �� ����
- �ֿ� ����
    1) ��� ���� ����
    - ��� �����͸� �����ϴµ� ���Ǹ�, VS�� PS���� ������ �� �ִ�
    - ��� ���� ������ register(b#)�� ����Ͽ� ����
    
    2) �ؽ�ó ����
    - �̹��� �����͸� �����ϴµ� ���
    - �ؽ�ó ���� ������ register(t#)�� ����Ͽ� ����

    3) ���÷� ����
    - �ؽ�ó�� ���ø��ϴ� �� ���
    - ���÷� ������ register(s#)�� ����Ͽ� ����

cbuffer = Constant Buffer : ��� ����
- ���̴����� ����ϴ� ���� ������ �����ϴµ� ���
- �� ��� ���� �������Ϳ��� �� ���� ��� ���۸� �Ҵ��� �� ����
- ��� ���� ���ο��� ���� ���� ������ ������ �� �ִ�

Sampler : ���÷�
- �ؽ�ó���� �ȼ� ���� �������� ����� �����ϴ� ��ü
- �ؽ�ó ���ø��� �ؽ�ó �̹����� ���� �ؼ� ���� ����ϴ� �۾�
  (�ؼ� : �ؽ�ó�� �ȼ� ��)
*/