#pragma once
#include "Renderer.h"

#include <wrl/client.h>

#include <string>
#include <d2d1.h>

#include "Misc/Type.h"
#include "Resource/Resource.h"

class UITexture : public Resource
{
    GENERATED_BODY(UITexture, Resource)
    
public:
    UITexture();
    virtual ~UITexture() override = default;

    virtual bool Load(const std::wstring& kPath) override;

    inline Microsoft::WRL::ComPtr<ID2D1Bitmap> GetTexture() const { return bitmap_; }
    
    inline Type::uint32 GetWidth() const { return width_; }
    inline Type::uint32 GetHeight() const { return height_; }

private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap_;
    
    Type::uint32 width_;
    Type::uint32 height_;
    
};
