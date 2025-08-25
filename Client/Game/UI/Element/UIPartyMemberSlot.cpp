#include "pch.h"
#include "UIPartyMemberSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "UIContextMenu.h"
#include "UIPopup.h"
#include "Windows/DX/UISprite.h"
#include "Windows/DX/Renderer.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Subsystems/SessionSubsystem.h"
#include <CustomPacket.h>

#include "UI/UIInGameState.h"

UIPartyMemberSlot::UIPartyMemberSlot(const std::wstring& name)
    : UIContainer(name),
    card_bg_(nullptr),
    portrait_(nullptr),
    level_text_(nullptr),
    name_text_(nullptr),
    hp_value_text_(nullptr),
    is_self_(false),
    am_party_host_(false),
    is_slot_host_(false),
    is_empty_(true),
    timer_(0.f),
    hp_effect_ratio_(0.f)
{
    SetSize({150.f, 200.f});
}

void UIPartyMemberSlot::Init()
{
    UISprite* panel_sprite  = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* player_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

    card_bg_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    card_bg_->SetDrawMode(UIImage::DrawMode::kSliced);
    card_bg_->SetSprite(panel_sprite, L"Panel_0");
    card_bg_->SetIgnoreRayCast(true);

    portrait_ = AddChild<UIImage>(UIImage::StaticClass(), L"Portrait");
    portrait_->SetSprite(player_sprite, L"UIPlayerSheet_0");
    portrait_->SetIgnoreRayCast(true);

    level_text_ = AddChild<UIText>(UIText::StaticClass(), L"LevelText");
    level_text_->SetColor(Math::Color::White);

    hp_value_text_ = AddChild<UIText>(UIText::StaticClass(), L"HPValueText");
    hp_value_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    hp_value_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    hp_value_text_->SetColor(Math::Color::White);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_text_->SetColor(Math::Color::White);

    Math::Vector2 sz = GetSize();
    float inner_w    = sz.x - 2.f * kPadding;
    float portrait_h = sz.y * kPortraitRatio;

    card_bg_->SetRelativePosition({0.f, 0.f});
    card_bg_->SetSize(sz);

    portrait_->SetRelativePosition({kPadding, kPadding});
    portrait_->SetSize({inner_w, portrait_h});

    level_text_->SetRelativePosition({kPadding + 3.f, kPadding + 3.f});
    level_text_->SetSize({120.f, 18.f});

    float bar_x = kPadding + kBarSidePadding;
    float bar_y = kPadding + portrait_h + kElementGap;
    float bar_w = inner_w - 2.f * kBarSidePadding;

    hp_value_text_->SetRelativePosition({bar_x, bar_y - 1.f});
    hp_value_text_->SetSize({bar_w, kBarHeight});

    name_text_->SetRelativePosition({6.f, bar_y + kBarHeight + kElementGap});
    name_text_->SetSize({sz.x - 12.f, 20.f});

    bar_pos_  = {bar_x, bar_y};
    bar_size_ = {bar_w, kBarHeight};

    SetEmpty(true);
    UIContainer::Init();
}

void UIPartyMemberSlot::SetEmpty(bool empty)
{
    is_empty_ = empty;
    if (portrait_)    portrait_->SetActive(!empty);
    if (level_text_)  level_text_->SetActive(!empty);
    if (name_text_)   name_text_->SetActive(!empty);
    if (hp_value_text_) hp_value_text_->SetActive(!empty);
}

void UIPartyMemberSlot::Update(const PartyMemberInfo& info, bool is_host, bool is_self, bool is_party_host)
{
    if (info.hp != info_.hp || info.max_hp != info_.max_hp) timer_ = 0.f;

    info_ = info;
    is_self_ = is_self;
    is_slot_host_ = is_host;
    am_party_host_ = is_party_host;

    std::wstring name = is_host ? (L"[방장]" + info.name) : info.name;
    name_text_->SetText(name);
    level_text_->SetText(L"레벨 " + std::to_wstring(info.lv));
    
    name_text_->SetColor(Math::Color::White);
    hp_value_text_->SetText(std::to_wstring(info_.hp) + L"/" + std::to_wstring(info_.max_hp));
    hp_value_text_->SetColor(Math::Color::White);

    portrait_->SetColor(Math::Color::HexToColor(info.body_color));
    SetEmpty(false);
}

void UIPartyMemberSlot::Render()
{
    if (!is_empty_ && info_.max_hp > 0 )
    {
        Math::Vector2 sz = GetSize();
        float inner_w    = sz.x - 2.f * kPadding;
        float portrait_h = sz.y * kPortraitRatio;

        float bar_x = kPadding + kBarSidePadding;
        float bar_y = kPadding + portrait_h + kElementGap;
        float bar_w = inner_w - 2.f * kBarSidePadding;

        float ratio = std::clamp(static_cast<float>(info_.hp) / static_cast<float>(info_.max_hp), 0.f, 1.f);
        Math::Vector2 abs = GetAbsolutePosition();
        Math::Vector2 pos = abs + Math::Vector2{bar_x, bar_y};

        Renderer* r = Renderer::Get();
        r->DrawSolidRoundBox(pos, {bar_w, kBarHeight}, Math::Color::Gray);
        r->DrawSolidRoundBox(pos, {bar_w * hp_effect_ratio_, kBarHeight}, Math::Color::White);
        r->DrawSolidRoundBox(pos, {bar_w * ratio, kBarHeight}, Math::Color::Red);
    }
    UIContainer::Render();
}

void UIPartyMemberSlot::Tick(float dt)
{
    UIContainer::Tick(dt);
    if (is_empty_) return;

    float hp_ratio = (info_.max_hp > 0) ? (static_cast<float>(info_.hp) / static_cast<float>(info_.max_hp)) : 0.f;
    hp_ratio = std::clamp(hp_ratio, 0.f, 1.f);
    if (timer_ < 1.f)
    {
        float t = timer_ / 1.f;
        t = t * t * (3.f - 2.f * t);
        hp_effect_ratio_ = Math::Lerp(hp_effect_ratio_, hp_ratio, t);
        timer_ += dt;
    }
    else hp_effect_ratio_ = hp_ratio;
}

bool UIPartyMemberSlot::OnMouseButton(const Math::Vector2& pos, MouseButton button, bool is_pressed, double ts)
{
    bool handled = UIContainer::OnMouseButton(pos, button, is_pressed, ts);
    if (is_empty_) return handled;

    if (is_pressed && button == MouseButton::kRight && IsInRange(pos) && am_party_host_ && !is_self_)
    {
        if (UIInGameState* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
        {
            UIContextMenu* menu = state->GetContextMenu();
            menu->Clear();
            menu->AddItem(L"방장 위임", [this]() { OnClickDelegate(); });
            menu->AddItem(L"강퇴",      [this]() { OnClickKick(); });
            menu->Show(pos);
        }
    }
    return handled;
}

void UIPartyMemberSlot::OnClickKick()
{
    UIPopup::PopupParam param;
    param.caption = L"정말로 " + info_.name + L" 님을 강퇴하시겠습니까?";
    param.option  = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [this](const std::wstring&, UIPopup::PopupOption opt)->bool
    {
        if (opt == UIPopup::PopupOption::Yes)
        {
            PartyKickRequest packet;
            packet.member_id = info_.character_id;
            SessionSubsystem::Get()->SendPacket(packet);
        }
        return true;
    };
    UIPopup::ShowPopup(param);
}

void UIPartyMemberSlot::OnClickDelegate()
{
    UIPopup::PopupParam param;
    param.caption = L"정말로 " + info_.name + L" 님에게 방장을 위임하시겠습니까?";
    param.option  = UIPopup::PopupOption::Yes | UIPopup::PopupOption::No;
    param.callback = [this](const std::wstring&, UIPopup::PopupOption opt)->bool
    {
        if (opt == UIPopup::PopupOption::Yes)
        {
            PartyDelegateRequest packet;
            packet.member_id = info_.character_id;
            SessionSubsystem::Get()->SendPacket(packet);
        }
        return true;
    };
    UIPopup::ShowPopup(param);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIPartyMemberSlot>("UIPartyMemberSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
