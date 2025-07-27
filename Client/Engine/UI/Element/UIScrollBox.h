#pragma once
#include "UIMask.h"

class UIScrollBox : public UIMask
{
    GENERATED_BODY(UIScrollBox, UIMask)
    
public:
    UIScrollBox(const std::wstring& name);
    virtual ~UIScrollBox() override = default;

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    
    virtual bool OnScroll(const Math::Vector2& position, const Math::Vector2& delta) override;

private:
    UIContainer* container_;

    float target_offset_;
    float offset_;
    float min_offset_;
    float max_offset_;
    
};
