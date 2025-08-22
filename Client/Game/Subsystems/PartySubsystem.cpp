#include "pch.h"
#include "PartySubsystem.h"

#include "GameInstance.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UIPartyPanel.h"

PartySubsystem::PartySubsystem() :
    members_(),
    party_id_(0),
    host_member_id_(0)
{
}

void PartySubsystem::AddOrUpdateMember(const PartyMemberInfo& info)
{
    members_[info.player_id] = info;
    UpdateUIAddOrUpdate(members_[info.player_id]);
}

void PartySubsystem::DeleteMember(uint32_t id)
{
    members_.erase(id);
    UpdateUIRemove(id);
}

void PartySubsystem::UpdateMemberStat(uint32_t id, PartyStatType stat, const std::wstring& value)
{
    auto it = members_.find(id);
    if (it == members_.end()) return;

    try
    {
        switch (stat)
        {
        case PartyStatType::kHP: it->second.hp = std::stoi(value); break;
        case PartyStatType::kMaxHP: it->second.max_hp = std::stoi(value); break;
        case PartyStatType::kLv: it->second.lv = std::stoi(value); break;
        default: break;
        }
    }
    catch (...)
    {
    }

    UpdateUIAddOrUpdate(it->second);
}

void PartySubsystem::Clear()
{
    members_.clear();
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        if (auto* panel = state->FindElement<UIPartyPanel>(L"PartyPanel"))
            panel->Clear();
    }
    SetPartyID(0);
    SetHostMemberID(0);
    SetPartyName(L"");
}

PartySubsystem* PartySubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PartySubsystem>();
}

void PartySubsystem::UpdateUIAddOrUpdate(const PartyMemberInfo& info)
{
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        if (auto* panel = state->FindElement<UIPartyPanel>(L"PartyPanel"))
            panel->AddOrUpdateMember(info);
    }
}

void PartySubsystem::UpdateUIRemove(uint32_t id)
{
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        if (auto* panel = state->FindElement<UIPartyPanel>(L"PartyPanel"))
            panel->RemoveMember(id);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<PartySubsystem>("PartySubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
