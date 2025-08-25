#include "pch.h"
#include "Party.h"

#include <CustomPacket.h>

#include "Player.h"
#include "Map/PlayerCharacter.h"
#include <ranges>

Party::Party(uint32_t party_id_) :
    party_id_(party_id_),
    host_id_(0)
{
}

void Party::SendPacket(const Net::IPacket& packet, uint32_t exclusion_member)
{
    std::vector<std::shared_ptr<Player>> recipients;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto& [id, member] : members_)
        {
            if (id == exclusion_member) continue;
            if (member.player) recipients.push_back(member.player);
        }
    }

    for (auto& p : recipients)
    {
        p->SendPacket(packet);
    }
}

bool Party::Contains(uint32_t character_id) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return members_.find(character_id) != members_.end();
}

bool Party::Contains(const std::shared_ptr<Player>& player) const
{
    if (!player) return false;
    auto character = player->GetPlayerCharacter();
    if (!character) return false;
    return Contains(character->GetObjectID());
}

void Party::AddPlayer(const std::shared_ptr<Player>& player)
{
    if (!player) return;

    PartyJoinPacket join_packet;
    PartyMemberChangedPacket change_pkt;

    PartyMemberInfo new_info{};
    auto new_char = player->GetPlayerCharacter();
    if (!new_char) return;

    new_info.character_id = new_char->GetObjectID();
    new_info.name = new_char->GetName();
    new_info.body_color = new_char->GetBodyColor();
    new_info.lv = new_char->GetLv();
    new_info.hp = new_char->GetHP();
    new_info.max_hp = new_char->GetMaxHP();

    bool already_member = false;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        already_member = members_.contains(new_info.character_id);
        members_[new_info.character_id] = { new_info, player };

        if (host_id_ == 0)
            host_id_ = new_info.character_id;

        join_packet.party_name = GetPartyName();
        join_packet.party_id = GetPartyID();
        join_packet.host_id = GetHost();
        for (auto& [id, member] : members_)
        {
            join_packet.members.push_back(member.info);
        }
    }

    change_pkt.change = already_member ? PartyMemberChangeType::kUpdate : PartyMemberChangeType::kJoin;
    change_pkt.member = new_info;

    player->SendPacket(join_packet);
    SendPacket(change_pkt, new_info.character_id);
}


int Party::GetPlayerCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return static_cast<int>(members_.size());
}

std::vector<std::shared_ptr<Player>> Party::GetPlayers() const
{
    std::vector<std::shared_ptr<Player>> snapshot;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto& [id, member] : members_)
        {
            if (member.player) snapshot.push_back(member.player);
        }
    }
    return snapshot;
}

void Party::DelegateHost(uint32_t new_host_id)
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!members_.contains(new_host_id))
            return;
        host_id_ = new_host_id;
    }

    PartyInfoChangedPacket packet;
    packet.type = PartyInfoType::kHostID;
    packet.value = std::to_wstring(host_id_);
    SendPacket(packet);
}

void Party::DeleteMember(uint32_t character_id)
{
    PartyMemberInfo info;
    std::shared_ptr<Player> removed_player;
    bool host_changed = false;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = members_.find(character_id);
        if (it == members_.end()) return;
        info = it->second.info;
        removed_player = it->second.player;
        members_.erase(it);
        if (host_id_ == character_id)
        {
            if (!members_.empty())
                host_id_ = members_.begin()->first;
            else
                host_id_ = 0;
            host_changed = true;
        }
    }

    PartyMemberChangedPacket member_change_packet;
    member_change_packet.change = PartyMemberChangeType::kLeave;
    member_change_packet.member = info;
    SendPacket(member_change_packet, character_id);

    if (removed_player)
    {
        if (auto pc = removed_player->GetPlayerCharacter())
            pc->SetPartyID(0);
        PartyLeavePacket leave_packet;
        removed_player->SendPacket(leave_packet);
    }

    if (host_changed)
    {
        PartyInfoChangedPacket packet;
        packet.type = PartyInfoType::kHostID;
        packet.value = std::to_wstring(host_id_);
    }
}