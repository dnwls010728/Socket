#pragma once
#include "CommonObject.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/UIContainer.h"

#include <vector>

class UISkillTooltip;
class UIButton;
class UIScrollBox;
class UIImage;
class UISkillSlot;

class UISkillWindow : public UIContainer
{
    GENERATED_BODY(UISkillWindow, UIContainer)

public:
    UISkillWindow(const std::wstring& name);
    virtual ~UISkillWindow() override = default;

    UIImage* GetDraggingIcon() const { return dragging_icon_; }
    UISkillTooltip* GetTooltip() const { return tooltip_; }

protected:
    virtual void Init() override;
    virtual void Uninit() override;

    virtual bool OnDragBegin(const Math::Vector2&) override;
    virtual bool OnDrag(const Math::Vector2&, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2&) override;

private:
    void Refresh(const std::vector<SkillInfo>& skills);
    void OnSkillUpdate(const EventData& data);

    UIScrollBox* scroll_box_;
    std::vector<UISkillSlot*> slots_;
    UIImage* dragging_icon_;
    UISkillTooltip* tooltip_;
};
