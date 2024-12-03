#pragma once
#include <DWrite.h>

#include "Math/Color.h"
#include "UI/Widget.h"

enum class TextAnchor : Type::uint8
{
    kLeft = (0x01<<0),
    kRight = (0x01<<1),
    kTop = (0x01<<2),
    kBottom = (0x01<<3),
    kCenter = (0x01<<4),
    kMiddle = (0x01<<5)
};

ENUM_CLASS_FLAGS(TextAnchor)

class Text : public Widget
{
    SHADER_CLASS_HELPER(Text)
    GENERATED_BODY(Text, Widget)

public:
    Text(const std::wstring& kName);
    virtual ~Text() override = default;

    void SetAlignment(TextAnchor anchor);

    FORCEINLINE void SetText(const std::wstring& text) { text_ = text; }
    FORCEINLINE const std::wstring& GetText() const { return text_; }

    FORCEINLINE void SetFontFamily(const std::wstring& font_family) { font_family_ = font_family; }
    FORCEINLINE const std::wstring& GetFontFamily() const { return font_family_; }

    FORCEINLINE void SetColor(Math::Color color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }

    static const TextAnchor kUpperLeft;
    static const TextAnchor kUpperCenter;
    static const TextAnchor kUpperRight;
    static const TextAnchor kMiddleLeft;
    static const TextAnchor kMiddleCenter;
    static const TextAnchor kMiddleRight;
    static const TextAnchor kLowerLeft;
    static const TextAnchor kLowerCenter;
    static const TextAnchor kLowerRight;

protected:
    virtual void Render() override;

    virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;

private:
    std::wstring text_;
    std::wstring font_family_;

    DWRITE_TEXT_ALIGNMENT text_alignment_;
    DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment_;

    Math::Color color_;
    
};
