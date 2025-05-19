#include "pch.h"
#include "UIInventorySlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UIInventorySlot::UIInventorySlot() :
    slot_id_(0),
    i_icon_(nullptr),
    t_count_(nullptr)
{
    size_ = { 32.f, 32.f };
}

void UIInventorySlot::Init()
{
    UIContainer::Init();

    i_icon_ = AddChild<UIImage>(UIImage::StaticClass());
    i_icon_->SetSize(size_);
    
    UITexture* icon = AssetManager::Get()->Load<UITexture>(L"UI\\Item\\Error.png");
    i_icon_->SetTexture(icon);
    i_icon_->SetIgnoreInteraction(true);
    
    t_count_ = AddChild<UIText>(UIText::StaticClass());
    t_count_->SetSize(size_);
    t_count_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
    t_count_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    t_count_->SetText(L"0");
    t_count_->SetIgnoreInteraction(true);
}

void UIInventorySlot::Render()
{
    Math::Vector2 parent_position = parent_ ? parent_->GetPosition() : Math::Vector2::Zero();
    Math::Vector2 position = parent_position + position_;
    Renderer::Get()->DrawBox(position, size_, Math::Color::Red);
    
    UIContainer::Render();
}

bool UIInventorySlot::OnDragBegin(const Math::Vector2& position)
{
    Logger::Print(L"UIInventorySlot::OnDragBegin: %u", slot_id_);
    return true;
}

bool UIInventorySlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    i_icon_->SetPosition((position - position_) - (i_icon_->GetSize() * .5f));
    return true;
}

bool UIInventorySlot::OnDragEnd(const Math::Vector2& position)
{
    // Logger::Print(L"UIInventorySlot::OnDragEnd: %u", slot_id_);
    i_icon_->SetPosition(Math::Vector2::Zero());
    return true;
}

bool UIInventorySlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    UIInventorySlot* target_slot = dynamic_cast<UIInventorySlot*>(target);
    if (target_slot)
    {
        Logger::Print(L"UIInventorySlot::OnDrop: %u, Target: %u", slot_id_, target_slot->slot_id_);
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
