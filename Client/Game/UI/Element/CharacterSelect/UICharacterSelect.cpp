#include "pch.h"
#include "UICharacterSelect.h"

#include "UICharacterSlot.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/UISprite.h"

UICharacterSelect::UICharacterSelect(const std::wstring& name) :
    UIContainer(name),
    slots_()
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    for (int32_t i = 0; i < 2; ++i)
    {
        for (int32_t j = 0; j < 3; ++j)
        {
            UICharacterSlot* slot = AddChild<UICharacterSlot>(UICharacterSlot::StaticClass(), L"CharacterSlot" + std::to_wstring(i * 3 + j));
            slot->SetRelativePosition({ 10.f + j * 200.f, 10.f + i * 228.f });
            slots_.push_back(slot);
        }
    }
}

void UICharacterSelect::InitSlots(const std::vector<CharacterProfile>& profiles) const
{
    for (int32_t i = 0; i < profiles.size(); ++i)
    {
        slots_[i]->InitSlot(profiles[i]);
    }
}

void UICharacterSelect::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterSelect>("UICharacterSelect")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
