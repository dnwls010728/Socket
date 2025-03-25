#pragma once
#include "UI/Widget.h"

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
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
    
    };
}
