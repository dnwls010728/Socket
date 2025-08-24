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

void Party::AddOfflineMember(const PartyMemberInfo& info)
{
    std::lock_guard<std::mutex> lock(mtx_);
    members_[info.character_id] = { info, nullptr };
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
    new_info.lv = new_char->GetLv();
    new_info.hp = new_char->GetHP();
    new_info.max_hp = new_char->GetMaxHP();
    new_info.is_online = true;

    bool already_member = false;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        already_member = members_.contains(new_info.character_id);
        members_[new_info.character_id] = { new_info, player };

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

void Party::RemovePlayer(uint32_t character_id)
{
    PartyMemberInfo info;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = members_.find(character_id);
        if (it == members_.end())
            return;

        it->second.player.reset();
        it->second.info.is_online = false;
        info = it->second.info;
    }

    PartyMemberChangedPacket update_packet;
    update_packet.change = PartyMemberChangeType::kUpdate;
    update_packet.member = info;
    SendPacket(update_packet, character_id);
}

void Party::RemovePlayer(const std::shared_ptr<Player>& player)
{
    if (!player) return;
    auto pc = player->GetPlayerCharacter();
    if (!pc) return;
    RemovePlayer(pc->GetObjectID());
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
