#pragma once

class Text : public SingletonBase<Text>
{
public:
    friend SingletonBase<Text>;

private:
    Text();
    ~Text();

public:
    void BeginDraw();
    void EndDraw();

    ID2D1DeviceContext* GetDC() { return deviceContext; }

    void CreateBackBuffer();
    void DeleteBackBuffer();

    void RenderText(wstring& text, RECT& rect);
    void RenderText(wstring& text, RECT& rect, Color color);
    void RenderText(wstring text, Vector2 pos, Color color, float size);
    void RenderText(wstring text, Vector2 pos, Color color, float size, bool left = false);
    void DrawLine(Vector2 start, Vector2 end, float strokeWidth);
    void SkewRectangle(RECT rect, D2D1::ColorF color, float angle, float angle2);
    int GetStringWidth(wstring text, float size);

private:
    ID2D1Factory1* factory = nullptr;
    IDWriteFactory* writeFactory = nullptr;

    ID2D1Device* device = nullptr;
    ID2D1DeviceContext* deviceContext = nullptr;

    ID2D1Bitmap1* bitmap = nullptr;
    IDXGISurface* surface = nullptr;

    ID2D1SolidColorBrush* brush = nullptr;
    IDWriteTextFormat* format = nullptr;

    bool beginDraw;
};