#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIBuffIcon : public UIContainer
{
    GENERATED_BODY(UIBuffIcon, UIContainer)
    
public:
    UIBuffIcon(const std::wstring& name);
    virtual ~UIBuffIcon() override = default;

    void Init(int32_t id, float expire_time);

protected:
    virtual void Tick(float delta_time) override;

private:
    float GetRemainingTime() const;

    UIImage* background_;
    UIImage* icon_;
    UIText* text_;

    float expire_time_;
    
};
