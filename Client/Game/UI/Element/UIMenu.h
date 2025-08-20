#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UISlider;
class UIImage;

class UIMenu : public UIContainer
{
    GENERATED_BODY(UIMenu, UIContainer)
    
public:
    UIMenu(const std::wstring& name);
    virtual ~UIMenu() override = default;

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    void OnMaster(float value);
    void OnBGM(float value);
    void OnSE(float value);
    void OnMobSE(float value);
    void OnSkillSE(float value);
    
    UIImage* background_;

    UIText* title_text_;
    UIText* master_text_;
    UIText* bgm_text_;
    UIText* se_text_;
    UIText* mob_se_text_;
    UIText* skill_se_text_;
    UIText* master_percent_text_;
    UIText* bgm_percent_text_;
    UIText* se_percent_text_;
    UIText* mob_se_percent_text_;
    UIText* skill_se_percent_text_;

    UISlider* master_slider_;
    UISlider* bgm_slider_;
    UISlider* se_slider_;
    UISlider* mob_se_slider_;
    UISlider* skill_se_slider_;
    
};
