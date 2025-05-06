#include "pch.h"
#include "NameTag.h"

#include <numeric>

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::NameTag::NameTag(const std::wstring& name) :
    Widget(name),
    text_(L"Unknown"),
    advances_(),
    total_advance_(0.f)
{
}

void UI::NameTag::SetText(const std::wstring& text)
{
    text_ = text;
    
    Renderer* renderer = Renderer::Get();
    renderer->GetTextAdvances(text_, L"NanumBarunGothic", 12.f, advances_);

    total_advance_ = std::accumulate(advances_.begin(), advances_.end(), 0.f);
}

std::shared_ptr<UI::NameTag> UI::NameTag::Create(const std::wstring& name)
{
    return std::make_shared<NameTag>(name);
}

void UI::NameTag::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    Math::Rect rect = GetRect(
        position_,
        {total_advance_ + 32.f, 32.f}
    );

    Math::Vector2 pivot_position = GetPivotPosition(rect);

    renderer->DrawSolidBox(window, rect, pivot_position, Math::Color(0, 0, 0, 100));
    renderer->DrawString(window, text_, rect, pivot_position, Math::Color::White, 0.f, L"NanumBarunGothic", 12.f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::NameTag>("UI::NameTag")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
