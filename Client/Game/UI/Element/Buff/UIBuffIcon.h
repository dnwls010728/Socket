#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIItemTooltip;
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
    
    virtual bool OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta) override;
    
    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;

private:
    float GetRemainingTime() const;
    
    UIItemTooltip* tooltip_;

    UIImage* background_;
    UIImage* icon_;
    UIText* text_;

    int32_t id_;

    float expire_time_;
    
};
