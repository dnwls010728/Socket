#include "pch.h"
#include "UIInventorySlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UIInventorySlot::UIInventorySlot(const std::wstring& name) :
    UIContainer(name),
    i_icon_(nullptr),
    t_count_(nullptr),
    item_id_(0)
{
    size_ = { 32.f, 32.f };
}

void UIInventorySlot::UpdateSlot(uint32_t item_id, uint32_t count)
{
    if (item_id_ != item_id)
    {
        UITexture* icon = AssetManager::Get()->Load<UITexture>(L"UI\\Item\\" + std::to_wstring(item_id) + L".png");
        if (icon) i_icon_->SetTexture(icon);
        item_id_ = item_id;
    }

    t_count_->SetText(std::to_wstring(count));
    t_count_->SetActive(count > 1);
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
    return true;
}

bool UIInventorySlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    UIInventorySlot* target_slot = dynamic_cast<UIInventorySlot*>(target);
    if (!target_slot) return false;

    uint32_t target_item_id = target_slot->GetItemID();
    if (target_item_id == 0) return false;
    
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
