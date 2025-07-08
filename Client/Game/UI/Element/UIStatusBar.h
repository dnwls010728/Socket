#pragma once
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIStatusBar : public UIContainer
{
    GENERATED_BODY(UIStatusBar, UIContainer)
    
public:
    UIStatusBar(const std::wstring& name);
    virtual ~UIStatusBar() override = default;

protected:
    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Render() override;

private:
    void OnEvent(const EventData& data);
    
    UIText* lv_text_;
    UIText* name_text_;
    UIText* hp_text_;
    UIText* exp_text_;

    uint32_t hp_;
    uint32_t max_hp_;
    uint32_t exp_;
    uint32_t max_exp_;
    
};
