#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIInventorySlot : public UIContainer
{
    GENERATED_BODY(UIInventorySlot, UIContainer)
    
public:
    UIInventorySlot();
    virtual ~UIInventorySlot() override = default;

    FORCEINLINE void SetSlotID(uint32_t slot_id) { slot_id_ = slot_id; }
    FORCEINLINE uint32_t GetSlotID() const { return slot_id_; }

protected:
    virtual void Init() override;
    virtual void Render() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;

private:
    uint32_t slot_id_;

    UIImage* i_icon_;
    UIText* t_count_;
    
};
