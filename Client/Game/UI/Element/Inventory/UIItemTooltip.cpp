#include "pch.h"
#include "UIItemTooltip.h"

#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIItemTooltip::UIItemTooltip(const std::wstring& name) :
    UIContainer(name)
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
    item_name_->SetSize({ 100.f, 30.f });
    item_name_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    item_name_->SetColor(Math::Color::White);
    
    item_description_ = AddChild<UIText>(UIText::StaticClass(), L"ItemDescription");
    item_description_->SetRelativePosition({ 102.f, 30.f });
    item_description_->SetSize({ 210.f, 92.f });
    item_description_->SetColor(Math::Color::White);
}

void UIItemTooltip::Set(uint32_t item_id)
{
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

    if (item_data)
    {
        item_name_->SetText(item_data->name);
        item_description_->SetText(item_data->desc);
    }
    else
    {
        item_name_->SetText(L"아이템 없음");
        item_description_->SetText(L"아이템 정보를 불러올 수 없습니다.");
    }
}

void UIItemTooltip::Init()
{
    UIContainer::Init();

    Math::Vector2 a = GetSize();

    frame_->SetSize(GetSize());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIItemTooltip>("UIItemTooltip")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
