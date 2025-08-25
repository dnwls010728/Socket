#pragma once
#include "UI/UIContainer.h"
#include <CommonObject.h>

class UIImage;
class UIText;
class UIContextMenu;

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
    UIImage* card_bg_ = nullptr;
    UIImage* portrait_ = nullptr;
    UIText* level_text_ = nullptr;
    UIText* name_text_ = nullptr;
    UIText* hp_value_text_ = nullptr;
    UIContextMenu* context_menu_ = nullptr;

    PartyMemberInfo info_{};
    bool is_self_ = false;
    bool is_slot_host_ = false;
    bool am_party_host_ = false;
    bool is_empty_ = true;

    Math::Vector2 bar_pos_{};
    Math::Vector2 bar_size_{};
    float timer_ = 0.f;
    float hp_effect_ratio_ = 0.f;
};
