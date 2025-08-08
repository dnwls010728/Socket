#pragma once

#include "Math/Color.h"
#include "UI/UIElement.h"

class UISprite;

class UIImage : public UIElement
{
    GENERATED_BODY(UIImage, UIElement)
    
public:
    enum class DrawMode
    {
        kSimple,
        kSliced
    };
    
    UIImage(const std::wstring& name);
    virtual ~UIImage() override = default;

    void SetSprite(UISprite* ui_sprite, const std::wstring& frame_name);
    void SetSprite(UISprite* ui_sprite, uint64_t frame_index = 0);

    FORCEINLINE UISprite* GetSprite() const { return ui_sprite_; }

    FORCEINLINE uint64_t GetFrameIndex() const { return frame_index_; }

    FORCEINLINE void SetDrawMode(DrawMode draw_mode) { draw_mode_ = draw_mode; }
    FORCEINLINE DrawMode GetDrawMode() const { return draw_mode_; }

    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }

protected:
    virtual void Render() override;

    UISprite* ui_sprite_;

    uint64_t frame_index_;

    DrawMode draw_mode_;

    Math::Color color_;
    
};
