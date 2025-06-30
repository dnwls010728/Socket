#include "pch.h"
#include "UILoginState.h"

#include "Asset/AssetManager.h"
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
    image->SetSize({1366.f, 768.f});
    image->SetSprite(sprite);

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
