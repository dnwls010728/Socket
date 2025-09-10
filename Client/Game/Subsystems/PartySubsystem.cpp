#include "pch.h"
#include "PartySubsystem.h"

#include "GameInstance.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UIPartyPanel.h"
#include "UI/Element/UIPartyWindow.h"

PartySubsystem::PartySubsystem() :
    members_(),
    party_id_(0),
    host_member_id_(0)
{
}

void PartySubsystem::AddOrUpdateMember(const PartyMemberInfo& info)
{
    members_[info.character_id] = info;
    UpdateUIAddOrUpdate(members_[info.character_id]);
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
        state->GetPartyPanel()->Clear();
        state->GetPartyWindow()->Clear();
    }
    SetPartyID(0);
    SetHostMemberID(0);
    SetPartyName(L"");
}

void PartySubsystem::Join(uint32_t id, const std::wstring& party_name)
{
    if (party_id_ == id) return;

    Clear();
    SetPartyID(id);
    SetPartyName(party_name);

    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->GetPartyPanel()->SetActive(true);
        state->GetPartyWindow()->Clear();
        state->GetPartyWindow()->SetActive(false);
    }
}

void PartySubsystem::Leave()
{
    if (party_id_ == 0) return;

    Clear();

    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->GetPartyPanel()->SetActive(false);
        state->GetPartyWindow()->SetActive(false);
    }
}

PartySubsystem* PartySubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PartySubsystem>();
}

void PartySubsystem::UpdateUIAddOrUpdate(const PartyMemberInfo& info)
{
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->GetPartyPanel()->AddOrUpdateMember(info);
        state->GetPartyWindow()->AddOrUpdateMember(info);
    }
}

void PartySubsystem::UpdateUIRemove(uint32_t id)
{
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->GetPartyPanel()->RemoveMember(id);
        state->GetPartyWindow()->RemoveMember(id);
    }
}

void PartySubsystem::SetHostMemberID(uint32_t host_member_id)
{
    host_member_id_ = host_member_id;
    if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->GetPartyWindow()->SetHost(host_member_id_);
    }
}

void PartySubsystem::RedrawUI()
{
    auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState());
    if (!state) return;
    
    if (IsJoinedParty())
    {
        auto* party_panel = state->GetPartyWindow();
        if (party_panel)
        {
            party_panel->Clear();
            party_panel->SetHost(GetHostMemberID());
            for (const auto& member : GetMembers())
            {
                party_panel->AddOrUpdateMember(member.second);
            }
            party_panel->SetActive(true);
        }

        auto* party_window = state->GetPartyWindow();
        if (party_window)
        {
            party_window->Clear();
            party_window->SetHost(GetHostMemberID());
            for (const auto& member : GetMembers())
            {
                party_window->AddOrUpdateMember(member.second);
            }
        }
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
