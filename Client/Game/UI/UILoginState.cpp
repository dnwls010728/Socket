#include "pch.h"
#include "UILoginState.h"

#include "Asset/AssetManager.h"
#include "Element/UILogin.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UILoginState::UILoginState()
{
}

void UILoginState::Init()
{
    UISprite* sprite = AssetManager::Get()->Load<UISprite>(L"UI\\LoginBackground.png");

    UIImage* image = AddElement<UIImage>(UIImage::StaticClass(), L"LoginBackground");
    image->SetSize({1366.f, 768.f});
    image->SetSprite(sprite, L"LoginBackground_0");
    image->SetIgnoreRayCast(true);

    UILogin* login = AddElement<UILogin>(UILogin::StaticClass(), L"Login");
    login->SetAbsolutePosition({514.f, 265.f});
    login->SetSize({338.f, 238.f});
    
    UIState::Init();

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
