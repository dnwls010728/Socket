#include "pch.h"
#include "OutlineRenderer.h"

#include "ConstantBuffer.h"

OutlineRenderer::OutlineRenderer(const Microsoft::WRL::ComPtr<ID2D1Brush>& outline_brush, const Microsoft::WRL::ComPtr<ID2D1Brush>& fill_brush, float stroke) :
    ref_count(1),
    outline_brush_(outline_brush),
    fill_brush_(fill_brush),
    stroke_(stroke)
{
}

HRESULT OutlineRenderer::QueryInterface(const IID& riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;
    if (riid == __uuidof(IUnknown) ||
        riid == __uuidof(IDWritePixelSnapping) ||
        riid == __uuidof(IDWriteTextRenderer))
    {
        *ppvObject = static_cast<IDWriteTextRenderer*>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG OutlineRenderer::AddRef()
{
    return ref_count.fetch_add(1, std::memory_order_relaxed) + 1;
}

ULONG OutlineRenderer::Release()
{
    ULONG prev_count = ref_count.fetch_sub(1, std::memory_order_acquire);
    if (prev_count == 1)
    {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete this;
        return 0;
    }
    
    return prev_count - 1;
}

HRESULT OutlineRenderer::IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled)
{
    if (!isDisabled) return E_POINTER;
    *isDisabled = FALSE;
    return S_OK;
}

HRESULT OutlineRenderer::GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform)
{
    if (!transform) return E_POINTER;
    auto* device_context = static_cast<ID2D1DeviceContext*>(clientDrawingContext);
    if (!device_context) return E_INVALIDARG;

    D2D1_MATRIX_3X2_F matrix;
    device_context->GetTransform(&matrix);
    transform->m11 = matrix._11;
    transform->m12 = matrix._12;
    transform->m21 = matrix._21;
    transform->m22 = matrix._22;
    transform->dx = matrix._31;
    transform->dy = matrix._32;
    return S_OK;
}

HRESULT OutlineRenderer::GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip)
{
    if (!pixelsPerDip) return E_POINTER;
    auto* device_context = static_cast<ID2D1DeviceContext*>(clientDrawingContext);
    if (!device_context)
    {
        *pixelsPerDip = 1.f;
        return S_OK;
    }

    FLOAT scale = 1.f;
    device_context->GetDpi(&scale, &scale);
    *pixelsPerDip = scale / 96.f;
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

    sink->SetFillMode(D2D1_FILL_MODE_WINDING);

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
    hr = factory->CreateStrokeStyle(props, nullptr, 0, stroke_style.GetAddressOf());
    if (FAILED(hr)) return hr;
    
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
