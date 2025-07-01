#include "pch.h"
#include "UILoginState.h"

#include "Asset/AssetManager.h"
#include "Element/UIDamageNumber.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UILoginState::UILoginState()
{
}

void UILoginState::Init()
{
    UIState::Init();

    UISprite* sprite = AssetManager::Get()->Load<UISprite>(L"UI\\LoginBackground.png");

    UIImage* image = AddElement<UIImage>(UIImage::StaticClass(), L"LoginBackground");
    image->SetSize({1.f, 1.f});
    image->SetSprite(sprite, L"LoginBackground_0");

    UIDamageNumber* damage_number = AddElement<UIDamageNumber>(UIDamageNumber::StaticClass(), L"DamageNumber");
    damage_number->SetSize({5.f, 5.f});

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
