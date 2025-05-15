#pragma once
#include "Math/Color.h"
#include "UI/Widget.h"

namespace UI_OLD
{
    class TextBox : public Widget
    {
        SHADER_CLASS_HELPER(TextBox)
        GENERATED_BODY(TextBox, Widget)
        
    public:
        TextBox(const std::wstring& kName);
        virtual ~TextBox() override = default;

        FORCEINLINE void SetText(const std::wstring& kText) { text_ = kText; }
        FORCEINLINE const std::wstring& GetText() const { return text_; }

        FORCEINLINE void SetColor(const Math::Color& kColor) { color_ = kColor; }
        FORCEINLINE const Math::Color& GetColor() const { return color_; }

        static std::shared_ptr<TextBox> Create(const std::wstring& kName);

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        std::wstring text_;

        Math::Color color_;
    
    };
}
