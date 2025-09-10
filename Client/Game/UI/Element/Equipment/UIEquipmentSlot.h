#pragma once
#include "UI/UIContainer.h"

class UIImage;
class UIEquipment;

class UIEquipmentSlot : public UIContainer
{
    GENERATED_BODY(UIEquipmentSlot, UIContainer)
    
public:
    UIEquipmentSlot(const std::wstring& name);
    virtual ~UIEquipmentSlot() override = default;
    
    void UpdateSlot(uint32_t item_id);
    void ResetSlot();
    
    FORCEINLINE void SetSlotID(uint32_t slot_id) { slot_id_ = slot_id; }
    FORCEINLINE uint32_t GetSlotID() const { return slot_id_; }

protected:
    virtual void Init() override;
    
    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    virtual bool OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;

    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;

private:
    UIImage* background_;
    UIImage* icon_;
    
    uint32_t slot_id_;
    uint32_t item_id_;
    
    float last_time_;
    
};
