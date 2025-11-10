#include "pch.h"
#include "UIEquipmentSlot.h"

#include <CustomPacket.h>

#include "UIEquipment.h"
#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Sprite.h"
#include "Windows/DX/UISprite.h"

UIEquipmentSlot::UIEquipmentSlot(const std::wstring& name) :
    UIContainer(name),
    slot_id_(0),
    item_id_(0),
    last_time_(0.f)
{
    SetSize({32.f, 32.f});

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetRelativePosition(Math::Vector2(4.f, 4.f));
    icon_->SetSize(GetSize() - Math::Vector2(8.f, 8.f));
    icon_->SetIgnoreRayCast(true);
}

void UIEquipmentSlot::UpdateSlot(uint32_t item_id)
{
    item_id_ = item_id;
    if (item_id <= 0) return;
    
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
        
        icon_->SetSprite(ui_sprite, frame_index);
    });
}

void UIEquipmentSlot::ResetSlot()
{
    item_id_ = 0;
    icon_->SetSprite(nullptr, L"");
}

void UIEquipmentSlot::Init()
{
    background_->SetSize(GetSize());

    UIContainer::Init();
}

bool UIEquipmentSlot::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    bool result = UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (button != MouseButton::kLeft || !is_pressed) return result;
    if (item_id_ == 0) return result;

    if (timestamp - last_time_ < .2f)
    {
        auto inventory = PlayerSubsystem::Get()->GetInventory();
        if (!inventory) return true;

        uint32_t free_slot = inventory->FindFreeSlot(InventoryType::kEquip);

        UnequipItemPacket packet;
        packet.first_slot = slot_id_;
        packet.second_slot = free_slot;
        SessionSubsystem::Get()->SendPacket(packet);

        last_time_ = 0.f;
        return true;
    }

    last_time_ = timestamp;
    return true;
}

bool UIEquipmentSlot::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    return UIContainer::OnMouseMotion(position, delta);
}

bool UIEquipmentSlot::OnMouseEnter()
{
    return UIContainer::OnMouseEnter();
}

bool UIEquipmentSlot::OnMouseLeave()
{
    return UIContainer::OnMouseLeave();
}

bool UIEquipmentSlot::OnDragBegin(const Math::Vector2& position)
{
    return UIContainer::OnDragBegin(position);
}

bool UIEquipmentSlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    return UIContainer::OnDrag(position, delta);
}

bool UIEquipmentSlot::OnDragEnd(const Math::Vector2& position)
{
    return UIContainer::OnDragEnd(position);
}

bool UIEquipmentSlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    return UIContainer::OnDrop(position, target);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIEquipmentSlot>("UIEquipmentSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
