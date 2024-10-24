#pragma once
#include <memory>

#include "UI/Widget.h"

class UITexture;

namespace UI
{
    enum class DrawMode
    {
        Simple,
        Sliced
    };
    
    class Image : public Widget
    {
        SHADER_CLASS_HELPER(Image)
        GENERATED_BODY(Image, Widget)
        
    public:
        Image(const std::wstring& kName);
        virtual ~Image() override = default;

        inline void SetTexture(UITexture* texture) { texture_ = texture; }
        inline void SetDrawMode(DrawMode draw_mode) { draw_mode_ = draw_mode; }
        inline void SetSlice9Rect(const Math::Rect& kSlice9Rect) { slice9_rect_ = kSlice9Rect; }

    protected:
        virtual void Render() override;

    private:
        UITexture* texture_;

        DrawMode draw_mode_;

        Math::Rect slice9_rect_;
        
    };
}
