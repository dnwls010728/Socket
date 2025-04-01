#include "pch.h"
#include "EditableTextBox.h"

UI::EditableTextBox::EditableTextBox(const std::wstring& kName) :
    Widget(kName)
{
}

std::shared_ptr<UI::EditableTextBox> UI::EditableTextBox::Create(const std::wstring& kName)
{
    return std::make_shared<EditableTextBox>(kName);
}

bool UI::EditableTextBox::OnKey(Type::uint16 key_code, bool is_pressed)
{
    return true;
}

bool UI::EditableTextBox::OnChar(wchar_t character)
{
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::EditableTextBox>("UI::EditableTextBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
