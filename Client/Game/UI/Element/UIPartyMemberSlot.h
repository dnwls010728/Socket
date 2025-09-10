#pragma once
#include "UI/UIContainer.h"
#include <CommonObject.h>

class UIImage;
class UIText;

class UIPartyMemberSlot : public UIContainer
{
    GENERATED_BODY(UIPartyMemberSlot, UIContainer)
public:
    UIPartyMemberSlot(const std::wstring& name);
    virtual ~UIPartyMemberSlot() override = default;

    static constexpr float kPadding        = 10.f;
    static constexpr float kPortraitRatio  = 0.56f;
    static constexpr float kBarHeight      = 14.f;
    static constexpr float kBarSidePadding = 6.f;
    static constexpr float kElementGap     = 6.f;

    void Update(const PartyMemberInfo& info, bool is_host, bool is_self, bool is_party_host);
    void SetEmpty(bool empty);
    const PartyMemberInfo& GetInfo() const { return info_; }

protected:
    virtual void Init() override;
    virtual void Render() override;
    virtual void Tick(float dt) override;
    virtual bool OnMouseButton(const Math::Vector2& pos, MouseButton button, bool is_pressed, double ts) override;

private:
    void OnClickKick();
    void OnClickDelegate();

private:
    UIImage* card_bg_;
    UIImage* portrait_;
    UIText* level_text_;
    UIText* name_text_;
    UIText* hp_value_text_;

    PartyMemberInfo info_;
    bool is_self_;
    bool is_slot_host_;
    bool am_party_host_;
    bool is_empty_ ;

    Math::Vector2 bar_pos_;
    Math::Vector2 bar_size_;
    float timer_;
    float hp_effect_ratio_;
};
