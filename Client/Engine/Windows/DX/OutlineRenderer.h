#pragma once
#include <DWrite.h>
#include <wrl/client.h>

struct ID2D1Brush;

class OutlineRenderer : public IDWriteTextRenderer
{
public:
    OutlineRenderer(const Microsoft::WRL::ComPtr<ID2D1Brush>& outline_brush, const Microsoft::WRL::ComPtr<ID2D1Brush>& fill_brush, float stroke);
    ~OutlineRenderer() = default;
    
    virtual HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
    virtual ULONG AddRef() override;
    virtual ULONG Release() override;
    virtual HRESULT IsPixelSnappingDisabled(void* clientDrawingContext, BOOL* isDisabled) override;
    virtual HRESULT GetCurrentTransform(void* clientDrawingContext, DWRITE_MATRIX* transform) override;
    virtual HRESULT GetPixelsPerDip(void* clientDrawingContext, FLOAT* pixelsPerDip) override;
    virtual HRESULT DrawGlyphRun(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode, const DWRITE_GLYPH_RUN* glyphRun, const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription, IUnknown* clientDrawingEffect) override;
    virtual HRESULT DrawUnderline(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, const DWRITE_UNDERLINE* underline, IUnknown* clientDrawingEffect) override;
    virtual HRESULT DrawStrikethrough(void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY, const DWRITE_STRIKETHROUGH* strikethrough, IUnknown* clientDrawingEffect) override;
    virtual HRESULT DrawInlineObject(void* clientDrawingContext, FLOAT originX, FLOAT originY, IDWriteInlineObject* inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown* clientDrawingEffect) override;

private:
    ULONG ref_;
    
    Microsoft::WRL::ComPtr<ID2D1Brush> outline_brush_;
    Microsoft::WRL::ComPtr<ID2D1Brush> fill_brush_;

    float stroke_;
};
