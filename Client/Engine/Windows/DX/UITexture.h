#pragma once
#include "Renderer.h"

#include <wrl/client.h>

#include <string>
#include <d2d1.h>

#include "Asset/Asset.h"

class UITexture : public Asset
{
    GENERATED_BODY(UITexture, Asset)
    
public:
    UITexture();
    virtual ~UITexture() override = default;

    virtual bool Load(const std::wstring& kPath) override;

    FORCEINLINE Microsoft::WRL::ComPtr<ID2D1Bitmap> GetTexture() const { return bitmap_; }
    
    FORCEINLINE uint32_t GetWidth() const { return width_; }
    FORCEINLINE uint32_t GetHeight() const { return height_; }

    FORCEINLINE void SetSlice9Rect(const Math::Rect& kRect) { slice9_rect_ = kRect; }
    FORCEINLINE const Math::Rect& GetSlice9Rect() const { return slice9_rect_; }

private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap_;
    
    uint32_t width_;
    uint32_t height_;

    Math::Rect slice9_rect_;
    
};
