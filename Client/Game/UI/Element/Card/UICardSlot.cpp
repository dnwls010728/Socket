#include "pch.h"
#include "UICardSlot.h"

#include "UI/Element/UIText.h"
#include "Subsystems/DataSubsystem.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/UISprite.h"

UICardSlot::UICardSlot(const std::wstring& name):
    UIButton(name),
    desc_(nullptr),
    card_id_(0)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");
    SetSprite(UIButton::State::kNormal, panel_sprite, L"ButtonSheet_0");
    SetSprite(UIButton::State::kHover, panel_sprite, L"ButtonSheet_1");
    SetSprite(UIButton::State::kPressed, panel_sprite, L"ButtonSheet_2");
    SetSprite(UIButton::State::kDisabled, panel_sprite, L"ButtonSheet_3");
    SetDrawMode(UIImage::DrawMode::kSliced);
    SetTextColor(Math::Color::White);
    
    desc_ = AddChild<UIText>(UIText::StaticClass(), L"Desc");
    desc_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    desc_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    desc_->SetColor(Math::Color::White);
    desc_->SetIgnoreRayCast(true);

    name_ = AddChild<UIText>(UIText::StaticClass(), L"Name");
    name_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    name_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_->SetColor(Math::Color::White);
    name_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetIgnoreRayCast(true);
}

void UICardSlot::Init()
{
    UIButton::Init();
}

void UICardSlot::SetSize(const Math::Vector2& size)
{
    UIButton::SetSize(size);
    UpdateLayout(size);
}

void UICardSlot::UpdateLayout(const Math::Vector2& size)
{
    constexpr float kIconWidth   = 80.f;
    constexpr float kIconHeight  = 80.f;
    constexpr float kNameHeight  = 20.f;
    constexpr float kDescHeight  = 20.f;

    constexpr float kSpacingIcon = 40.f;
    constexpr float kSpacingName = 40.f;
    constexpr float kSpacingDesc = 10.f;
    
    Math::Vector2 icon_size = {kIconWidth, kIconHeight};
    Math::Vector2 icon_pos  = {(size.x - icon_size.x) * 0.5f, kSpacingIcon};
    icon_->SetSize(icon_size);
    icon_->SetRelativePosition(icon_pos);
    
    Math::Vector2 name_pos  = {0.f, icon_pos.y + icon_size.y + kSpacingName};
    Math::Vector2 name_size = {size.x, kNameHeight};
    name_->SetRelativePosition(name_pos);
    name_->SetSize(name_size);
    
    Math::Vector2 desc_pos  = {0.f, name_pos.y + name_size.y + kSpacingDesc};
    Math::Vector2 desc_size = {size.x, kDescHeight};
    desc_->SetRelativePosition(desc_pos);
    desc_->SetSize(desc_size);
}

void UICardSlot::SetCard(const CardSelectInfo& card)
{
    const CardData* data = DataSubsystem::Get()->GetCard(card.card_id);
    if (!data) return;
    
    card_id_ = card.card_id;
    std::wstring card_name = L"LV. " + std::to_wstring(card.level);
    name_->SetText(card_name + L" " + data->name);
    desc_->SetText(data->desc);
    icon_->SetSprite(AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png"));
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UICardSlot>("UICardSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
