#pragma once
#include "UI/Widget.h"

class UITexture;

namespace UI
{
    class TestWidget : public Widget
    {
        SHADER_CLASS_HELPER(Widget)
        GENERATED_BODY(Widget)
        
    public:
        TestWidget(const std::wstring& kName);
        virtual ~TestWidget() override = default;

        static std::shared_ptr<TestWidget> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
        
        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;

        UITexture* texture_;
    
    };
}
