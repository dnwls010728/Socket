#pragma once
#include "UI/UIElement.h"

class UIDamageNumber : public UIElement
{
    GENERATED_BODY(UIDamageNumber, UIElement)
    
public:
    UIDamageNumber(const std::wstring& name);
    virtual ~UIDamageNumber() override = default;

    FORCEINLINE void SetDamage(int64_t damage) { damage_ = damage; }

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

private:
    UISprite* number_sprite_;
    UISprite* miss_sprite_;

    int64_t damage_;
    
    float count_timer_;
    float fade_timer_;
    float alpha_;
    
};
