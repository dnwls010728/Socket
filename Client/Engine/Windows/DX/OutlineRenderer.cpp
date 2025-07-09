#include "pch.h"
#include "OutlineRenderer.h"

#include "ConstantBuffer.h"

OutlineRenderer::OutlineRenderer(const Microsoft::WRL::ComPtr<ID2D1Brush>& outline_brush, const Microsoft::WRL::ComPtr<ID2D1Brush>& fill_brush, float stroke) :
    outline_brush_(outline_brush),
    fill_brush_(fill_brush),
    stroke_(stroke)
{
}

HRESULT OutlineRenderer::QueryInterface(const IID& riid, void** ppvObject)
{
    return S_OK;
}

ULONG OutlineRenderer::AddRef()
{
    return 0;
}

ULONG OutlineRenderer::Release()
{
    return 0;
}

HRESULT OutlineRenderer::IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled)
{
    return S_OK;
}

HRESULT OutlineRenderer::GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform)
{
    return S_OK;
}

HRESULT OutlineRenderer::GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip)
{
    return S_OK;
}

HRESULT OutlineRenderer::DrawGlyphRun(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, const DWRITE_GLYPH_RUN* glyphRun, const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription, IUnknown* clientDrawingEffect)
{
    ID2D1DeviceContext* device_context = static_cast<ID2D1DeviceContext*>(clientDrawingContext);
    IDWriteFontFace* font_face = glyphRun->fontFace;

    Microsoft::WRL::ComPtr<ID2D1Factory> factory;
    device_context->GetFactory(factory.GetAddressOf());
    
    Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
    
    HRESULT hr = factory->CreatePathGeometry(path.GetAddressOf());
    if (FAILED(hr)) return hr;

    Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
    hr = path->Open(sink.GetAddressOf());
    if (FAILED(hr)) return hr;

    hr = font_face->GetGlyphRunOutline(
        glyphRun->fontEmSize,
        glyphRun->glyphIndices,
        glyphRun->glyphAdvances,
        glyphRun->glyphOffsets,
        glyphRun->glyphCount,
        glyphRun->isSideways,
        glyphRun->bidiLevel % 2,
        sink.Get()
    );
    if (FAILED(hr)) return hr;
    
    hr = sink->Close();
    if (FAILED(hr)) return hr;
    
    Microsoft::WRL::ComPtr<ID2D1TransformedGeometry> geo;
    hr = factory->CreateTransformedGeometry(
        path.Get(),
        D2D1::Matrix3x2F::Translation(baselineOriginX, baselineOriginY),
        &geo
    );
    if (FAILED(hr)) return hr;

    D2D1_STROKE_STYLE_PROPERTIES props = D2D1::StrokeStyleProperties(
        D2D1_CAP_STYLE_FLAT,
        D2D1_CAP_STYLE_FLAT,
        D2D1_CAP_STYLE_FLAT,
        D2D1_LINE_JOIN_ROUND,
        1.f
    );
    
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke_style;
    factory->CreateStrokeStyle(props, nullptr, 0, stroke_style.GetAddressOf());
    
    device_context->DrawGeometry(geo.Get(), outline_brush_.Get(), stroke_, stroke_style.Get());
    device_context->FillGeometry(geo.Get(), fill_brush_.Get());

    return S_OK;
}

HRESULT OutlineRenderer::DrawUnderline(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, const DWRITE_UNDERLINE* underline, IUnknown* clientDrawingEffect)
{
    return S_OK;
}

HRESULT OutlineRenderer::DrawStrikethrough(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, const DWRITE_STRIKETHROUGH* strikethrough, IUnknown* clientDrawingEffect)
{
    return S_OK;
}

HRESULT OutlineRenderer::DrawInlineObject(void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect)
{
    return S_OK;
}
