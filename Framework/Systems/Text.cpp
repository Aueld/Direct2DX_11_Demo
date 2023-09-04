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
    // DWRITE_FACTORY_TYPE_SHARED   : 공유 팩토리, 여러 프로세스 내 컴포넌트 간에 캐시된 글꼴 데이터의 재사용을 허용한다
    // DWRITE_FACTORY_TYPE_ISOLATED : 팩토리 객체가 격리되었다. 격리된 팩토리에서 생성된 객체는 다른 컴포넌트 내부 DirectWrite 상태와 상호작용 안함
    
    // __uuidof : 식에 연결된 GUID를 검색하는데 사용
    // GUID : Globally Unique Identifier
    // 전역 고유 식별자라는 의미
    // 128비트 정수로 컴퓨터 시스템에서 정보를 식별하는데 사용
    // GUID의 중복 가능성은 0이 아니지만 0에 가까움
    // 이미 생성됐는지, 다른 것과 중복되지 않는지 검사하는데 사용

    //IUknown : 컴포넌트 개체 모델(COM)의 기본 인터페이스
#pragma endregion

    D2D1_FACTORY_OPTIONS option;
    option.debugLevel    = D2D1_DEBUG_LEVEL_WARNING;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
    CHECK(hr);

    #pragma region CreateFactory
    // D2D1_DEBUG_LEVEL_WARNING : 오류 메시지와 경고를 디버그 계층으로 보낸다
    // D2D1CreateFactory : Direct2D 리소스를 생성하는데 사용할 수 있는 팩토리 개체를 생성하는 함수
    // D2D1_FACTORY_TYPE_MULTI_THREADED : 팩토리와 팩토리가 생성하는 객체에 대한 접근 및 쓰기를 위해 동기화를 제공하며,
    // 여러 스레드에서 안전하게 접근할 수 있도록 한다는 것
    #pragma endregion

    IDXGIDevice* dxgiDevice;
    hr = DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    CHECK(hr);

    #pragma region IDXGIDevice
    // IDXGIDevice : DXGI 객체 중 이미지 데이터를 생성하는 객체에 대한 파생 클래스를 구현
    // 다른 DXGI 객체에 엑세스해야 하는 DXGI 객체에서 사용하도록 설계
    // Direct3D를 사용하여 DXGI와 통신하지 않는 애플리케이션에 유용
    // QueryInterface : COM 객체가 특정 인터페이스를 구현하는지 확인하고 해당 인터페이스에 대한 포인터를 반환하는데 사용
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
    // D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS : Direct2D가 렌더링 작업을 여러 스레드에 분산하도록 지정
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
        L"돋움체",
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
            VARIABLE_PITCH | FF_MODERN, TEXT("돋움체"));

    HDC hdc  = ::GetDC(handle);
    holdFont = (HFONT)SelectObject(hdc, hFont);
    SIZE sz;
    GetTextExtentPoint32(hdc, text.c_str(), text.length(), &sz);
    
    DeleteObject(SelectObject(hdc, holdFont));
    ReleaseDC(handle, hdc);

    return sz.cx;
}