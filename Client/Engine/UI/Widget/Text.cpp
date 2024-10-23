#include "pch.h"
#include "Text.h"

#include "Level/World.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

UI::Text::Text(const std::wstring& kName) :
    Widget(kName),
    text_(),
    color_(Math::Color::White),
    text_alignment_(DWRITE_TEXT_ALIGNMENT_LEADING),
    paragraph_alignment_(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
{
}

void UI::Text::SetAlignment(TextAnchor alignment)
{
    switch (alignment)
    {
    case TextAnchor::kUpperLeft:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_LEADING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        break;
    case TextAnchor::kUpperCenter:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_CENTER;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        break;
    case TextAnchor::kUpperRight:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_TRAILING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        }
        break;
    case TextAnchor::kMiddleLeft:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_LEADING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        break;
    case TextAnchor::kMiddleCenter:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_CENTER;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        break;
    case TextAnchor::kMiddleRight:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_TRAILING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        }
        break;
    case TextAnchor::kLowerLeft:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_LEADING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }
        break;
    case TextAnchor::kLowerCenter:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_CENTER;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }
        break;
    case TextAnchor::kLowerRight:
        {
            text_alignment_ = DWRITE_TEXT_ALIGNMENT_TRAILING;
            paragraph_alignment_ = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        }
        break;
    }
}

void UI::Text::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    renderer->DrawString(window, text_, rect_, pivot_position, color_, angle_, L"Silver24", text_alignment_, paragraph_alignment_);
}
