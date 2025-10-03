#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UISkillWindow;
class UIImage;

class UISkillSlot : public UIContainer
{
    GENERATED_BODY(UISkillSlot, UIContainer)

public:
    UISkillSlot(const std::wstring& name);
    virtual ~UISkillSlot() override = default;

    void SetSkill(uint32_t skill_id, int32_t level, float cooldown);
    void Reset();

    FORCEINLINE uint32_t GetSkillID() const { return skill_id_; }
    FORCEINLINE UIImage* GetIcon() const { return icon_; }

    FORCEINLINE void SetOwner(UISkillWindow* owner) { owner_ = owner; }

protected:
    virtual void Init() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;
private:
    UISkillWindow* owner_;

    UIImage* background_;
    UIImage* icon_;

    UIText* name_text_;
    UIText* level_text_;

    uint32_t skill_id_;
    int32_t level_;
    float cooldown_;

    std::wstring description_;
};
