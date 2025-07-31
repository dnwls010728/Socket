#include "pch.h"
#include "UICharacterSelect.h"

#include "UICharacterSlot.h"
#include "Asset/AssetManager.h"
#include "UI/Element/UIButton.h"
#include "Windows/DX/UISprite.h"

UICharacterSelect::UICharacterSelect(const std::wstring& name) :
    UIContainer(name),
    slots_()
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

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

    delete_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"DeleteButton");
    delete_button_->SetRelativePosition({10.f, 488.f});
    delete_button_->SetSize({200.f, 53.f});
    delete_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    delete_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    delete_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    delete_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    delete_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    delete_button_->SetTextColor(Math::Color::White);
    delete_button_->SetText(L"캐릭터 삭제");

    new_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"NewButton");
    new_button_->SetRelativePosition({ 210.f, 488.f });
    new_button_->SetSize({ 200.f, 53.f });
    new_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    new_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    new_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    new_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    new_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    new_button_->SetTextColor(Math::Color::White);
    new_button_->SetText(L"캐릭터 생성");

    select_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"SelectButton");
    select_button_->SetRelativePosition({ 410.f, 488.f });
    select_button_->SetSize({ 200.f, 53.f });
    select_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    select_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    select_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    select_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    select_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    select_button_->SetTextColor(Math::Color::White);
    select_button_->SetText(L"게임 시작");
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
