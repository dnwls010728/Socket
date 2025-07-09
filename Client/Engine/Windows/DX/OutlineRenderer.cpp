#include "pch.h"
#include "OutlineRenderer.h"

#include "ConstantBuffer.h"

OutlineRenderer::OutlineRenderer(const Microsoft::WRL::ComPtr<ID2D1Brush>& outline_brush, const Microsoft::WRL::ComPtr<ID2D1Brush>& fill_brush, float stroke) :
    ref_(1),
    outline_brush_(outline_brush),
    fill_brush_(fill_brush),
    stroke_(stroke)
{
}

HRESULT OutlineRenderer::QueryInterface(const IID& riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;
    if (riid == __uuidof(IUnknown) ||
        riid == __uuidof(IDWriteTextRenderer) ||
        riid == __uuidof(IDWritePixelSnapping))
    {
        *ppvObject = this; AddRef(); return S_OK;
    }
    
    *ppvObject = nullptr; return E_NOINTERFACE;
}

ULONG OutlineRenderer::AddRef()
{
    return InterlockedIncrement(&ref_);
}

ULONG OutlineRenderer::Release()
{
    ULONG ref = InterlockedDecrement(&ref_);
    if (ref == 0) delete this;
    return ref;
}

HRESULT OutlineRenderer::IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled)
{
    if (!isDisabled) return E_POINTER;

    *isDisabled = FALSE;
    return S_OK;
}

HRESULT OutlineRenderer::GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform)
{
    if (!clientDrawingContext || !transform) return E_POINTER;

    ID2D1RenderTarget* render_target = static_cast<ID2D1RenderTarget*>(clientDrawingContext);
    render_target->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));

    return S_OK;
}

HRESULT OutlineRenderer::GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip)
{
    if (!clientDrawingContext || !pixelsPerDip) return E_POINTER;

    float dpi_x, dpi_y;
    static_cast<ID2D1RenderTarget*>(clientDrawingContext)->GetDpi(&dpi_x, &dpi_y);
    *pixelsPerDip = dpi_x / 96.f;
    
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

    device_context->DrawGeometry(geo.Get(), outline_brush_.Get(), stroke_);
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
