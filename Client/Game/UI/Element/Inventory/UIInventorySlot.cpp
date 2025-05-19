#include "pch.h"
#include "UIInventorySlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UIInventorySlot::UIInventorySlot() :
    i_icon_(nullptr),
    t_count_(nullptr),
    item_id_(0)
{
    size_ = { 32.f, 32.f };
}

void UIInventorySlot::UpdateSlot(int32_t item_id, int16_t count)
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

    i_icon_ = AddChild<UIImage>(UIImage::StaticClass());
    i_icon_->SetSize(size_);
    i_icon_->SetIgnoreInteraction(true);
    
    t_count_ = AddChild<UIText>(UIText::StaticClass());
    t_count_->SetSize(size_);
    t_count_->SetColor(Math::Color::White);
    t_count_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
    t_count_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    t_count_->SetIgnoreInteraction(true);
    t_count_->SetActive(false);
}

void UIInventorySlot::Render()
{
    Renderer::Get()->DrawBox(GetAbsolutePosition(), size_, Math::Color::Red);
    
    UIContainer::Render();
}

bool UIInventorySlot::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIInventorySlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    i_icon_->SetAbsolutePosition(position - (i_icon_->GetSize() * .5f));
    return true;
}

bool UIInventorySlot::OnDragEnd(const Math::Vector2& position)
{
    // Logger::Print(L"UIInventorySlot::OnDragEnd: %u", slot_id_);
    i_icon_->SetRelativePosition(Math::Vector2::Zero());
    return true;
}

bool UIInventorySlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    UIInventorySlot* target_slot = dynamic_cast<UIInventorySlot*>(target);
    if (target_slot)
    {
    }
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInventorySlot>("UIInventorySlot")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
