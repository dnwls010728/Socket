#include "pch.h"
#include "UIEquipTooltip.h"

#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIEquipTooltip::UIEquipTooltip(const std::wstring& name) :
    UITooltip(name)
{
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    frame_ = AddChild<UIImage>(UIImage::StaticClass(), L"Frame");
    frame_->SetSprite(panel_sprite, L"Panel_0");
    frame_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    icon_slot_ = AddChild<UIImage>(UIImage::StaticClass(), L"IconSlot");
    icon_slot_->SetRelativePosition({ 10.f, 30.f });
    icon_slot_->SetSize({ 82.f, 82.f });
    icon_slot_->SetSprite(panel_sprite, L"Panel_0");
    icon_slot_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    item_icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"ItemIcon");
    item_icon_->SetRelativePosition({ 20.f, 40.f });
    item_icon_->SetSize({ 62.f, 62.f });

    item_name_ = AddChild<UIText>(UIText::StaticClass(), L"ItemName");
    item_name_->SetRelativePosition({ 10.f, 0.f });
    item_name_->SetSize({ 240.f, 30.f });
    item_name_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    item_name_->SetColor(Math::Color::White);
    
    req_lv_ = AddChild<UIText>(UIText::StaticClass(), L"ReqLv");
    req_lv_->SetRelativePosition({ 10.f, 122.f });
    req_lv_->SetSize({ 240.f, 20.f });
    req_lv_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    req_lv_->SetColor({255, 230, 120, 255});
    
    max_hp_ = AddChild<UIText>(UIText::StaticClass(), L"MaxHP");
    max_hp_->SetRelativePosition({ 10.f, 162.f });
    max_hp_->SetSize({ 240.f, 20.f });
    max_hp_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    max_hp_->SetColor(Math::Color::White);
    
    atk_ = AddChild<UIText>(UIText::StaticClass(), L"ATK");;
    atk_->SetRelativePosition({ 10.f, 182.f });
    atk_->SetSize({ 240.f, 20.f });
    atk_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    atk_->SetColor(Math::Color::White);
    
    def_ = AddChild<UIText>(UIText::StaticClass(), L"DEF");;
    def_->SetRelativePosition({ 10.f, 202.f });
    def_->SetSize({ 240.f, 20.f });
    def_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    def_->SetColor(Math::Color::White);
    
    dig_ = AddChild<UIText>(UIText::StaticClass(), L"DIG");;
    dig_->SetRelativePosition({ 10.f, 222.f });
    dig_->SetSize({ 240.f, 20.f });
    dig_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    dig_->SetColor(Math::Color::White);
}

void UIEquipTooltip::Set(uint32_t item_id)
{
    UITooltip::Set(item_id);
    
    const ItemData* item_data = DataSubsystem::Get()->GetItem(item_id);
    if (!item_data) return;
    
    AssetManager::Get()->LoadAsync<UISprite>(item_data->ui_icon.path, [this, item_data](UISprite* ui_sprite)
    {
        int32_t frame_index = item_data->ui_icon.index;
        if (!ui_sprite)
        {
            static UISprite* kMissing = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\Missing.png");
            ui_sprite = kMissing;
            frame_index = 0;
        }
        
        item_icon_->SetSprite(ui_sprite, frame_index);
    });
    
    item_name_->SetText(item_data->name);
    req_lv_->SetText(L"요구 레벨: " + std::to_wstring(item_data->stat.req_lv));
    max_hp_->SetText(L"최대 HP: " + std::to_wstring(item_data->stat.max_hp));
    atk_->SetText(L"공격력: " + std::to_wstring(item_data->stat.atk));
    def_->SetText(L"방어력: " + std::to_wstring(item_data->stat.def));
    dig_->SetText(L"방어력 무시: " + std::to_wstring(item_data->stat.dig) + L"%");
}

void UIEquipTooltip::Init()
{
    UIContainer::Init();
    
    Math::Vector2 a = GetSize();

    frame_->SetSize(GetSize());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIEquipTooltip>("UIEquipTooltip")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
