#include "pch.h"
#include "UIInventorySlot.h"

#include <CustomPacket.h>

#include "Asset/AssetManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UIState.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIInventorySlot::UIInventorySlot(const std::wstring& name) :
    UIContainer(name),
    i_icon_(nullptr),
    t_count_(nullptr),
    slot_id_(0),
    item_id_(0)
{
    size_ = { 32.f, 32.f };
}

void UIInventorySlot::UpdateSlot(uint32_t item_id, uint32_t count)
{
    item_id_ = item_id;
    if (item_id > 0)
    {
        UISprite* ui_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\" + std::to_wstring(item_id) + L".png");
        if (ui_sprite) i_icon_->SetSprite(ui_sprite, std::to_wstring(item_id) + L"_0");

        t_count_->SetText(std::to_wstring(count));
    }
    else
    {
        i_icon_->SetSprite(nullptr, L"");
        t_count_->SetText(L"");
    }
}

void UIInventorySlot::Init()
{
    UIContainer::Init();

    i_icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    i_icon_->SetSize(size_);
    i_icon_->SetIgnoreRayCast(true);
    
    t_count_ = AddChild<UIText>(UIText::StaticClass(), L"Count");
    t_count_->SetSize(size_);
    t_count_->SetColor(Math::Color::White);
    t_count_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
    t_count_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    t_count_->SetIgnoreRayCast(true);
    t_count_->SetActive(false);
}

void UIInventorySlot::Render()
{
    Renderer::Get()->DrawBox(GetAbsolutePosition(), size_, Math::Color::Red);
    
    UIContainer::Render();
}

bool UIInventorySlot::OnDragBegin(const Math::Vector2& position)
{
    if (item_id_ == 0) return false;
    return true;
}

bool UIInventorySlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (item_id_ == 0) return false;
    
    i_icon_->SetAbsolutePosition(position - (i_icon_->GetSize() * .5f));
    t_count_->SetAbsolutePosition(position - (t_count_->GetSize() * .5f));
    return true;
}

bool UIInventorySlot::OnDragEnd(const Math::Vector2& position)
{
    if (item_id_ == 0) return false;
    i_icon_->SetRelativePosition(Math::Vector2::Zero());
    t_count_->SetRelativePosition(Math::Vector2::Zero());

    UIElement* element = UI::Get()->GetState()->RayCast(position);
    if (!element)
    {
        MoveItemRequest request;
        request.type = ItemMoveType::kDrop;
        request.src = slot_id_;
        SessionSubsystem::Get()->SendPacket(request);
    }
    
    return true;
}

bool UIInventorySlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    UIInventorySlot* target_slot = dynamic_cast<UIInventorySlot*>(target);
    if (!target_slot) return false;

    MoveItemRequest request;
    request.type = ItemMoveType::kMove;
    request.src = target_slot->GetSlotID();
    request.dest = slot_id_;
    SessionSubsystem::Get()->SendPacket(request);
    
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInventorySlot>("UIInventorySlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
