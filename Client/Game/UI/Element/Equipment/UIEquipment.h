#pragma once
#include "UI/UIContainer.h"

class UIImage;

class UIEquipment : public UIContainer
{
    GENERATED_BODY(UIEquipment, UIContainer)
    
public:
    UIEquipment(const std::wstring& name);
    virtual ~UIEquipment() override = default;

protected:
    virtual void Init() override;
    
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    UIImage* background_;
    
};
