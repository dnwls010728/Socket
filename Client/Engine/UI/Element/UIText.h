#pragma once
#include <DWrite.h>

#include "Math/Color.h"
#include "UI/UIElement.h"

class UIText : public UIElement
{
    GENERATED_BODY(UIText, UIElement)
    
public:
    UIText();
    virtual ~UIText() override = default;

    FORCEINLINE void SetText(const std::wstring& text) { text_ = text; }
    FORCEINLINE const std::wstring& GetText() const { return text_; }
    
    FORCEINLINE void SetFontName(const std::wstring& font_name) { font_name_ = font_name; }
    FORCEINLINE const std::wstring& GetFontName() const { return font_name_; }
    
    FORCEINLINE void SetFontSize(float font_size) { font_size_ = font_size; }
    FORCEINLINE float GetFontSize() const { return font_size_; }
    
    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }
    
    FORCEINLINE void SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment) { text_alignment_ = alignment; }
    FORCEINLINE DWRITE_TEXT_ALIGNMENT GetTextAlignment() const { return text_alignment_; }
    
    FORCEINLINE void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment) { paragraph_alignment_ = alignment; }
    FORCEINLINE DWRITE_PARAGRAPH_ALIGNMENT GetParagraphAlignment() const { return paragraph_alignment_; }

protected:
    virtual void Render(const Math::Vector2& parent_position) override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;

private:
    std::wstring text_;
    std::wstring font_name_;

    float font_size_;

    Math::Color color_;
    
    DWRITE_TEXT_ALIGNMENT text_alignment_;
    DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment_;
    
};
