#pragma once
#include <DWrite.h>

#include "Math/Color.h"
#include "UI/Widget.h"

namespace UI
{
    enum class TextAnchor
    {
        kUpperLeft,
        kUpperCenter,
        kUpperRight,
        kMiddleLeft,
        kMiddleCenter,
        kMiddleRight,
        kLowerLeft,
        kLowerCenter,
        kLowerRight
    };
    
    class Text : public Widget
    {
        SHADER_CLASS_HELPER(Text)
        GENERATED_BODY(Text, Widget)
        
    public:
        Text(const std::wstring& kName);
        virtual ~Text() override = default;

        void SetAlignment(TextAnchor alignment);

        FORCEINLINE void SetText(const std::wstring& kText) { text_ = kText; }
        FORCEINLINE void SetColor(const Math::Color& kColor) { color_ = kColor; }

    protected:
        virtual void Render() override;

    private:
        std::wstring text_;

        Math::Color color_;

        DWRITE_TEXT_ALIGNMENT text_alignment_;
        DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment_;
    
    };
}
