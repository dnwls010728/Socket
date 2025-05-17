#include "pch.h"
#include "UILoginState.h"

#include "UI/Element/UIText.h"

UILoginState::UILoginState()
{
}

void UILoginState::Init()
{
    UIState::Init();

    UIText* text = AddElement<UIText>(UIText::StaticClass());
    text->SetSize({100.f, 30.f});
    text->SetText(L"요소 테스트");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UILoginState>("UILoginState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
