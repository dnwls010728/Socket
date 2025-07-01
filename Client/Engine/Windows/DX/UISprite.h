#pragma once
#include "Renderer.h"

#include <wrl/client.h>

#include <string>
#include <d2d1.h>

#include "Asset/Asset.h"

class UISprite : public Asset
{
    GENERATED_BODY(UISprite, Asset)
    
public:
    enum class FilterMode : uint8_t
    {
        kPoint,
        kBilinear
    };

    enum class DrawMode : uint8_t
    {
        kSimple,
        kSliced
    };
    
    struct Frame
    {
        Math::Vector2 offset;
        Math::Vector2 size;
        Math::Vector2 border_min;
        Math::Vector2 border_max;
    };
    
    UISprite();
    virtual ~UISprite() override = default;

    virtual bool Load(const std::wstring& path) override;

    FORCEINLINE Microsoft::WRL::ComPtr<ID2D1Bitmap> GetTexture() const { return bitmap_; }
    
    FORCEINLINE uint32_t GetWidth() const { return width_; }
    FORCEINLINE uint32_t GetHeight() const { return height_; }

    FORCEINLINE void SetSlice9Rect(const Math::Rect& kRect) { slice9_rect_ = kRect; }
    FORCEINLINE const Math::Rect& GetSlice9Rect() const { return slice9_rect_; }

private:
    friend class Renderer;
    
    bool LoadBitmap(const std::wstring& path);
    
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap_;
    
    uint32_t width_;
    uint32_t height_;

    FilterMode filter_mode_;

    std::map<std::wstring, Frame> frames_;

    Math::Rect slice9_rect_;
    
};
