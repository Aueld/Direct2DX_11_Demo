#include "Framework.h"
#include "Text.h"

Text::Text()
{
    HRESULT hr;

    hr = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        (IUnknown**)&writeFactory
    );
    CHECK(hr);

    #pragma region DWriteCreateFactory
    // DWRITE_FACTORY_TYPE_SHARED   : ���� ���丮, ���� ���μ��� �� ������Ʈ ���� ĳ�õ� �۲� �������� ������ ����Ѵ�
    // DWRITE_FACTORY_TYPE_ISOLATED : ���丮 ��ü�� �ݸ��Ǿ���. �ݸ��� ���丮���� ������ ��ü�� �ٸ� ������Ʈ ���� DirectWrite ���¿� ��ȣ�ۿ� ����
    
    // __uuidof : �Ŀ� ����� GUID�� �˻��ϴµ� ���
    // GUID : Globally Unique Identifier
    // ���� ���� �ĺ��ڶ�� �ǹ�
    // 128��Ʈ ������ ��ǻ�� �ý��ۿ��� ������ �ĺ��ϴµ� ���
    // GUID�� �ߺ� ���ɼ��� 0�� �ƴ����� 0�� �����
    // �̹� �����ƴ���, �ٸ� �Ͱ� �ߺ����� �ʴ��� �˻��ϴµ� ���

    //IUknown : ������Ʈ ��ü ��(COM)�� �⺻ �������̽�
#pragma endregion

    D2D1_FACTORY_OPTIONS option;
    option.debugLevel    = D2D1_DEBUG_LEVEL_WARNING;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
    CHECK(hr);

    #pragma region CreateFactory
    // D2D1_DEBUG_LEVEL_WARNING : ���� �޽����� ��� ����� �������� ������
    // D2D1CreateFactory : Direct2D ���ҽ��� �����ϴµ� ����� �� �ִ� ���丮 ��ü�� �����ϴ� �Լ�
    // D2D1_FACTORY_TYPE_MULTI_THREADED : ���丮�� ���丮�� �����ϴ� ��ü�� ���� ���� �� ���⸦ ���� ����ȭ�� �����ϸ�,
    // ���� �����忡�� �����ϰ� ������ �� �ֵ��� �Ѵٴ� ��
    #pragma endregion

    IDXGIDevice* dxgiDevice;
    hr = DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    CHECK(hr);

    #pragma region IDXGIDevice
    // IDXGIDevice : DXGI ��ü �� �̹��� �����͸� �����ϴ� ��ü�� ���� �Ļ� Ŭ������ ����
    // �ٸ� DXGI ��ü�� �������ؾ� �ϴ� DXGI ��ü���� ����ϵ��� ����
    // Direct3D�� ����Ͽ� DXGI�� ������� �ʴ� ���ø����̼ǿ� ����
    // QueryInterface : COM ��ü�� Ư�� �������̽��� �����ϴ��� Ȯ���ϰ� �ش� �������̽��� ���� �����͸� ��ȯ�ϴµ� ���
    #pragma endregion

    hr = factory->CreateDevice(dxgiDevice, &device);
    CHECK(hr);

    hr = device->CreateDeviceContext
    (
        D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
        &deviceContext
    );
    CHECK(hr);

    #pragma region CreateDeviceContext
    // D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS : Direct2D�� ������ �۾��� ���� �����忡 �л��ϵ��� ����
    #pragma endregion

    CreateBackBuffer();
}

Text::~Text()
{
    DeleteBackBuffer();
}

void Text::BeginDraw()
{
    if (beginDraw)
        return;

    beginDraw = true;
    TXT->GetDC()->BeginDraw();
}

void Text::EndDraw()
{
    if (!beginDraw)
        return;

    beginDraw = false;
    TXT->GetDC()->EndDraw();
}

void Text::CreateBackBuffer()
{
    HRESULT hr;
    hr = GRAPHICS->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
    CHECK(hr);

    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96;
    bp.dpiY = 96;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = NULL;

    hr = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
    CHECK(hr);

    deviceContext->SetTarget(bitmap);

    hr = deviceContext->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &brush);
    assert(SUCCEEDED(hr));

    writeFactory->CreateTextFormat
    (
        L"����ü",
        NULL,
        DWRITE_FONT_WEIGHT_SEMI_LIGHT,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        15,
        L"ko",
        &format
    );
}

void Text::DeleteBackBuffer()
{
    deviceContext->SetTarget(NULL);

    SAFE_RELEASE(brush);
    SAFE_RELEASE(format);

    SAFE_RELEASE(bitmap);
    SAFE_RELEASE(surface);
}

void Text::RenderText(wstring& text, RECT& rect)
{
    D2D1_RECT_F rt;
    rt.left   = (float)rect.left;
    rt.top    = (float)rect.top;
    rt.right  = (float)rect.right;
    rt.bottom = (float)rect.bottom;

    deviceContext->DrawTextW(text.c_str(), text.length(), format, &rt, brush);
}

void Text::RenderText(wstring& text, RECT& rect, Color color)
{
    D2D1_RECT_F rt;
    rt.left   = (float)rect.left;
    rt.top    = (float)rect.top;
    rt.right  = (float)rect.right;
    rt.bottom = (float)rect.bottom;

    ID2D1SolidColorBrush* brush2;
    deviceContext->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b), &brush2);
    deviceContext->DrawTextW(text.c_str(), text.length(), format, &rt, brush2);
    SAFE_RELEASE(brush2);
}

void Text::RenderText(wstring text, Vector2 pos, Color color, float size)
{
    ID2D1SolidColorBrush* brush;
    deviceContext->CreateSolidColorBrush(D2D1::ColorF(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f), &brush);

    IDWriteTextLayout* layout = nullptr;
    HRESULT hr = writeFactory->CreateTextLayout
    (
        text.c_str(),
        text.length(),
        format,
        (float)text.length() * size,
        (float)size,
        &layout
    );
    CHECK(hr);

    DWRITE_TEXT_RANGE range;
    range.startPosition = 0;
    range.length = text.length();

    layout->SetFontSize((float)size, range);
    layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

    deviceContext->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, brush);

    SAFE_RELEASE(layout);
    SAFE_RELEASE(brush);
}

void Text::RenderText(wstring text, Vector2 pos, Color color, float size, bool left)
{
    ID2D1SolidColorBrush* brush;
    deviceContext->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b), &brush);

    IDWriteTextLayout* layout = nullptr;
    HRESULT hr = writeFactory->CreateTextLayout
    (
        text.c_str(),
        text.length(),
        format,
        (float)text.length() * size,
        (float)size,
        &layout
    );
    CHECK(hr);

    DWRITE_TEXT_RANGE range;
    range.startPosition = 0;
    range.length = text.length();

    layout->SetFontSize((float)size, range);
    if (left)
    {
        layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }
    else
    {
        layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    }

    deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

    deviceContext->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, brush);

    SAFE_RELEASE(layout);
    SAFE_RELEASE(brush);
}

void Text::DrawLine(Vector2 start, Vector2 end, float strokeWidth)
{
    ID2D1SolidColorBrush* brush;
    deviceContext->CreateSolidColorBrush(D2D1::ColorF(0, 1, 0), &brush);

    deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

    Vector2 startPos = start;
    Vector2 endPos = end;

    deviceContext->DrawLine(D2D1::Point2F(startPos.x, startPos.y), D2D1::Point2F(endPos.x, endPos.y), brush, strokeWidth);
    SAFE_RELEASE(brush);
}

void Text::SkewRectangle(RECT rect, D2D1::ColorF color, float angle, float angle2)
{
    D2D1_RECT_F rt;
    rt.left   = (float)rect.left;
    rt.top    = (float)rect.top;
    rt.right  = (float)rect.right;
    rt.bottom = (float)rect.bottom;

    D2D1_POINT_2F offset;
    offset.x = rect.left + (rect.right - rect.left) * 0.5f;
    offset.y = rect.top + (rect.bottom - rect.top) * 0.5f;

    ID2D1SolidColorBrush* brush;
    deviceContext->CreateSolidColorBrush(color, &brush);
    deviceContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, offset));
    deviceContext->SetTransform(D2D1::Matrix3x2F::Skew(angle2, 0, offset));
    deviceContext->FillRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom), brush);

    SAFE_RELEASE(brush);
}

int Text::GetStringWidth(wstring text, float size)
{
    HFONT hFont, holdFont;

    hFont = CreateFont((int)size, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1,
            VARIABLE_PITCH | FF_MODERN, TEXT("����ü"));

    HDC hdc  = ::GetDC(handle);
    holdFont = (HFONT)SelectObject(hdc, hFont);
    SIZE sz;
    GetTextExtentPoint32(hdc, text.c_str(), text.length(), &sz);
    
    DeleteObject(SelectObject(hdc, holdFont));
    ReleaseDC(handle, hdc);

    return sz.cx;
}