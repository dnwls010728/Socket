#pragma once
#include "Scancode.h"
#include "Subsystems/InputActions/KeyType.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;
class UISkillSlot;

class UIQuickSlot : public UIContainer
{
    GENERATED_BODY(UIQuickSlot, UIContainer)
    
public:
    UIQuickSlot(const std::wstring& name);
    virtual ~UIQuickSlot() override = default;

    void SetScancode(Scancode scancode);

protected:
    virtual void Init() override;
    
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;

private:
    UIImage* background_;
    UIImage* icon_;

    UIText* key_name_text_;
    UIText* count_text_;

    Scancode scancode_;

    KeyType key_type_;
    int32_t action_;

};
