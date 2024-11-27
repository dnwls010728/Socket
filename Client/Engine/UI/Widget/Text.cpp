#include "pch.h"
#include "Text.h"

#include "Logger.h"
#include "Level/World.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

const TextAnchor Text::kUpperLeft = TextAnchor::kTop | TextAnchor::kLeft;
const TextAnchor Text::kUpperCenter = TextAnchor::kTop | TextAnchor::kCenter;
const TextAnchor Text::kUpperRight = TextAnchor::kTop | TextAnchor::kRight;
const TextAnchor Text::kMiddleLeft = TextAnchor::kMiddle | TextAnchor::kLeft;
const TextAnchor Text::kMiddleCenter = TextAnchor::kMiddle | TextAnchor::kCenter;
const TextAnchor Text::kMiddleRight = TextAnchor::kMiddle | TextAnchor::kRight;
const TextAnchor Text::kLowerLeft = TextAnchor::kBottom | TextAnchor::kLeft;
const TextAnchor Text::kLowerCenter = TextAnchor::kBottom | TextAnchor::kCenter;
const TextAnchor Text::kLowerRight = TextAnchor::kBottom | TextAnchor::kRight;

Text::Text(const std::wstring& kName) :
    Widget(kName),
    text_(L"New Text"),
    font_family_(L"Nanum18"),
    text_alignment_(DWRITE_TEXT_ALIGNMENT_LEADING),
    paragraph_alignment_(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
    color_(Math::Color::White)
{
}

void Text::SetAlignment(TextAnchor anchor)
{
    if (EnumHasAnyFlags(anchor, TextAnchor::kLeft)) text_alignment_ = DWRITE_TEXT_ALIGNMENT_LEADING;
    else if (EnumHasAnyFlags(anchor, TextAnchor::kRight)) text_alignment_ = DWRITE_TEXT_ALIGNMENT_TRAILING;
    else text_alignment_ = DWRITE_TEXT_ALIGNMENT_CENTER;

    if (EnumHasAnyFlags(anchor, TextAnchor::kTop)) paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
    else if (EnumHasAnyFlags(anchor, TextAnchor::kBottom)) paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
    else paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
}

void Text::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    renderer->DrawString(window, text_, rect_, pivot_position, color_, angle_, font_family_, text_alignment_, paragraph_alignment_);
    
    Widget::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Text>("Text")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
