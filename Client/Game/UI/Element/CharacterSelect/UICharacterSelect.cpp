#include "pch.h"
#include "UICharacterSelect.h"

#include <CustomPacket.h>

#include "UICharacterSlot.h"
#include "Asset/AssetManager.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIState.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UICharacterCreate.h"
#include "Windows/DX/UISprite.h"

UICharacterSelect::UICharacterSelect(const std::wstring& name) :
    UIContainer(name),
    slots_(),
    selected_slot_id_(0)
{
    SetAbsolutePosition({373.f, 84.f});
    SetSize({620.f, 539.f});
    
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
    delete_button_->SetRelativePosition({10.f, 476.f});
    delete_button_->SetSize({200.f, 53.f});
    delete_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    delete_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    delete_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    delete_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    delete_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    delete_button_->SetTextColor(Math::Color::White);
    delete_button_->SetText(L"캐릭터 삭제");
    delete_button_->OnClick(this, &UICharacterSelect::OnDeleteCharacter);
    delete_button_->SetDisabled(true);

    new_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"NewButton");
    new_button_->SetRelativePosition({ 210.f, 476.f });
    new_button_->SetSize({ 200.f, 53.f });
    new_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    new_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    new_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    new_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    new_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    new_button_->SetTextColor(Math::Color::White);
    new_button_->SetText(L"캐릭터 생성");
    new_button_->OnClick(this, &UICharacterSelect::OnCreateCharacter);

    select_button_ = AddChild<UIButton>(UIButton::StaticClass(), L"SelectButton");
    select_button_->SetRelativePosition({ 410.f, 476.f });
    select_button_->SetSize({ 200.f, 53.f });
    select_button_->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    select_button_->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    select_button_->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    select_button_->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    select_button_->SetDrawMode(UIImage::DrawMode::kSliced);
    select_button_->SetTextColor(Math::Color::White);
    select_button_->SetText(L"게임 시작");
    select_button_->OnClick(this, &UICharacterSelect::OnCharacterSelected);
    select_button_->SetDisabled(true);
}

void UICharacterSelect::InitSlots(const std::vector<CharacterProfile>& profile)
{
    for (int32_t i = 0; i < profile.size(); ++i)
    {
        slots_[i]->InitSlot(this, i + 1, profile[i]);
    }

    if (profile.size() >= slots_.size()) new_button_->SetDisabled(true);
    else new_button_->SetDisabled(false);
}

void UICharacterSelect::RefreshSlots(const std::vector<CharacterProfile>& profile)
{
    for (int32_t i = 0; i < slots_.size(); ++i)
    {
        if (i < profile.size()) slots_[i]->InitSlot(this, i + 1, profile[i]);
        else slots_[i]->ClearSlot();
    }
    
    if (profile.size() >= slots_.size()) new_button_->SetDisabled(true);
    else new_button_->SetDisabled(false);

    selected_slot_id_ = 0;
}

void UICharacterSelect::Init()
{
    background_->SetSize(GetSize());
    UIContainer::Init();
    InitSlots(PlayerSubsystem::Get()->GetProfiles());
}

void UICharacterSelect::Render()
{
    UIContainer::Render();

    if (selected_slot_id_ > 0)
    {
        UICharacterSlot* slot = slots_[selected_slot_id_ - 1];
        Renderer::Get()->DrawRoundBox(slot->GetAbsolutePosition(), slot->GetSize(), {255, 211, 77, 242}, 5.f, 4.f);
    }
}

void UICharacterSelect::OnCreateCharacter()
{
    if (auto* state = UI::Get()->GetState())
    {
        state->RemoveElement(this);
        state->AddElement<UICharacterCreate>(UICharacterCreate::StaticClass(), L"CharacterCreate");
    }
}

void UICharacterSelect::OnDeleteCharacter() const
{
    UICharacterSlot* selected_slot = slots_[selected_slot_id_ - 1];
    if (selected_slot->GetCharacterID() == 0) return;
    
    DeleteCharacterRequest request;
    request.character_id = selected_slot->GetCharacterID();
    SessionSubsystem::Get()->SendPacket(request);

    delete_button_->SetDisabled(true);
    select_button_->SetDisabled(true);
}

void UICharacterSelect::OnSlotSelected(uint32_t slot_id)
{
    selected_slot_id_ = slot_id;

    if (delete_button_->IsDisabled()) delete_button_->SetDisabled(false);
    if (select_button_->IsDisabled()) select_button_->SetDisabled(false);
}

void UICharacterSelect::OnCharacterSelected() const
{
    UICharacterSlot* selected_slot = slots_[selected_slot_id_ - 1];
    if (selected_slot->GetCharacterID() == 0) return;

    SelectCharacterRequest request;
    request.character_id = selected_slot->GetCharacterID();
    SessionSubsystem::Get()->SendPacket(request);

    select_button_->SetDisabled(true);
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
