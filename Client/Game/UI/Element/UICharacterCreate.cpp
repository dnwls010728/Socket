#include "pch.h"
#include "UICharacterCreate.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UICharacterCreate::UICharacterCreate(const std::wstring& name) :
    UIContainer(name)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);
}

void UICharacterCreate::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterCreate>("UICharacterCreate")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
