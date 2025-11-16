#pragma once
#include "Scancode.h"
#include "Subsystems/InputActions/KeyType.h"
#include "Subsystems/InputActions/InputActions.h"
#include "CommonObject.h""
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;
class UISkillSlot;
class UIQuickBar;

class UIQuickSlot : public UIContainer
{
    GENERATED_BODY(UIQuickSlot, UIContainer)
    
public:
    UIQuickSlot(const std::wstring& name);
    virtual ~UIQuickSlot() override = default;
    
    void SetScancode(Scancode scancode);
    Scancode GetScancode() const { return scancode_; }

    void SetOwner(UIQuickBar* owner) { owner_ = owner; }
    UIQuickBar* GetOwner() const { return owner_; }

    KeyType GetKeyType() const { return key_type_; }
    int32_t GetAction() const { return action_; }

    void ClearMapping();
protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;
    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
private:
    void ApplyMapping(const InputActions::Mapping& mapping);
    void ApplySkillMapping(uint32_t skill_id);
    void ApplyItemMapping(uint32_t item_id);
    void UpdateCooldownVisual();
    void UpdateItemCountVisual();

    UIQuickBar* owner_;
    UIImage* background_;
    UIImage* icon_;

    UIText* key_name_text_;
    UIText* count_text_;
    UIText* cooldown_text_;

    Scancode scancode_;

    KeyType key_type_;
    int32_t action_;

    // Skill
    int32_t combo_index_;

};
