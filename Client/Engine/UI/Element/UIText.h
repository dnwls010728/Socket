#pragma once
#include <DWrite.h>

#include "Math/Color.h"
#include "UI/UIElement.h"

class UIText : public UIElement
{
    GENERATED_BODY(UIText, UIElement)
    
public:
    UIText(const std::wstring& name);
    virtual ~UIText() override = default;

    void SetText(const std::wstring& text);
    void SetFontName(const std::wstring& font_name);
    void SetFontSize(float font_size);
    
    FORCEINLINE const std::wstring& GetText() const { return text_; }
    
    FORCEINLINE const std::wstring& GetFontName() const { return font_name_; }
    
    FORCEINLINE float GetFontSize() const { return font_size_; }
    
    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }
    
    FORCEINLINE void SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment) { text_alignment_ = alignment; }
    FORCEINLINE DWRITE_TEXT_ALIGNMENT GetTextAlignment() const { return text_alignment_; }
    
    FORCEINLINE void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment) { paragraph_alignment_ = alignment; }
    FORCEINLINE DWRITE_PARAGRAPH_ALIGNMENT GetParagraphAlignment() const { return paragraph_alignment_; }

    FORCEINLINE const std::vector<float>& GetAdvances() const { return advances_; }
    FORCEINLINE float GetTotalAdvance() const { return total_advance_; }

protected:
    virtual void Render() override;

private:
    void UpdateAdvances();
    
    std::wstring text_;
    std::wstring font_name_;
    
    float font_size_;

    Math::Color color_;
    
    DWRITE_TEXT_ALIGNMENT text_alignment_;
    DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment_;

    std::vector<float> advances_;
    
    float total_advance_;
    
};
