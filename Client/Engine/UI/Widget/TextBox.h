#pragma once
#include "UI/Widget.h"

namespace UI
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

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        std::wstring text_;
    
    };
}
