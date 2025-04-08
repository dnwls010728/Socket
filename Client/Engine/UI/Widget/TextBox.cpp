#include "pch.h"
#include "TextBox.h"

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::TextBox::TextBox(const std::wstring& kName) :
    Widget(kName),
    text_(L"Text"),
    color_(Math::Color::White)
{
}

std::shared_ptr<UI::TextBox> UI::TextBox::Create(const std::wstring& kName)
{
    return std::make_shared<TextBox>(kName);
}

void UI::TextBox::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    renderer->DrawBox(window, GetRect(), GetPivotPosition(), Math::Color::Black, 0.f, 1.f);

    renderer->DrawString(window, text_, GetRect(), GetPivotPosition(), color_, 0.f, L"NanumBarunGothic", 18.f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::TextBox>("UI::TextBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
