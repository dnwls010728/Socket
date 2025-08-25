#pragma once
#include "UI/UIContainer.h"
#include <CommonObject.h>
#include <unordered_map>
#include <vector>

class UIPartyMemberSlot;
class UIButton;
class UIImage;
class UIText;

class UIPartyWindow : public UIContainer
{
    GENERATED_BODY(UIPartyWindow, UIContainer)
public:
    UIPartyWindow(const std::wstring& name);
    virtual ~UIPartyWindow() override = default;

    static constexpr int   kCols         = 4;
    static constexpr float kPad          = 12.f;
    static constexpr float kTopBarHeight = 30.f;
    static constexpr float kGapX         = 12.f;
    static constexpr float kGapY         = 12.f;
    static constexpr float kBottomPad    = 12.f;
    static constexpr int   kInitialSlots = 4;
    static constexpr int   kBatchSlots   = 4;
    static constexpr float kSlotWidth    = 150.f;
    static constexpr float kSlotHeight   = 200.f;

    void AddOrUpdateMember(const PartyMemberInfo& info);
    void RemoveMember(uint32_t id);
    void Clear();
    void SetHost(uint32_t id);

protected:
    virtual void Init() override;
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    void EnsureSlots(size_t required);
    void RefreshLayout();
    void RefreshMembers();
    void RefreshSlots();
    void OnClose();
    void OnCreateParty();
    void OnLeaveParty();

    std::unordered_map<uint32_t, PartyMemberInfo> members_;
    std::vector<UIPartyMemberSlot*> slots_;

    UIButton* close_button_;
    UIImage* background_;
    UIButton* create_button_;
    UIButton* leave_button_;
    UIText* empty_text_;
    UIText* party_name_text_;

    uint32_t host_id_;
};
