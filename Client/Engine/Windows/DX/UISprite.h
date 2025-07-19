#pragma once
#include "Renderer.h"

#include <wrl/client.h>

#include <string>
#include <d2d1.h>

#include "Asset/Asset.h"

struct UISpriteFrame
{
    Math::Vector2 offset;
    Math::Vector2 size;
    Math::Vector2 border_min;
    Math::Vector2 border_max;
};

namespace YAML
{
    template<>
    struct convert<UISpriteFrame>
    {
        static bool decode(const Node& node, UISpriteFrame& data)
        {
            if (!node.IsMap()) return false;
            data.offset.x = node["rect"]["x"].as<float>();
            data.offset.y = node["rect"]["y"].as<float>();
            data.size.x = node["rect"]["width"].as<float>();
            data.size.y = node["rect"]["height"].as<float>();
            data.border_min.x = node["border"]["left"].as<float>();
            data.border_min.y = node["border"]["top"].as<float>();
            data.border_max.x = node["border"]["right"].as<float>();
            data.border_max.y = node["border"]["bottom"].as<float>();
            return true;
        }
    };
}

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
    
    UISprite();
    virtual ~UISprite() override = default;

    virtual bool Load(const std::wstring& path) override;

    uint32_t GetWidth(const std::wstring& frame_name) const;
    uint32_t GetHeight(const std::wstring& frame_name) const;

    FORCEINLINE Microsoft::WRL::ComPtr<ID2D1Bitmap> GetSprite() const { return bitmap_; }
    
    FORCEINLINE uint32_t GetWidth() const { return width_; }
    FORCEINLINE uint32_t GetHeight() const { return height_; }
    
    FORCEINLINE const std::unordered_map<std::wstring, uint64_t>& GetFrameIndexes() const { return frame_indexes_; }
    FORCEINLINE const std::vector<UISpriteFrame>& GetFrames() const { return frames_; }

    FORCEINLINE void SetSlice9Rect(const Math::Rect& kRect) { slice9_rect_ = kRect; }
    FORCEINLINE const Math::Rect& GetSlice9Rect() const { return slice9_rect_; }

private:
    friend class Renderer;
    
    bool LoadBitmap(const std::wstring& path);
    
    Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap_;
    
    uint32_t width_;
    uint32_t height_;

    FilterMode filter_mode_;

    std::unordered_map<std::wstring, uint64_t> frame_indexes_;
    std::vector<UISpriteFrame> frames_;

    Math::Rect slice9_rect_;
    
};
