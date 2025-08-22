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

void Party::SendPacket(const Net::IPacket& packet, uint32_t exclusion_player)
{
    std::vector<std::shared_ptr<Player>> recipients;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        recipients.reserve(players_.size());
        for (auto& [id, player] : players_) {
            if (!player) continue;
            if (id != exclusion_player)
                recipients.push_back(player);
        }
    }

    for (auto& p : recipients) {
        p->SendPacket(packet);
    }
}

bool Party::Contains(uint32_t player_id) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return players_.find(player_id) != players_.end();
}

bool Party::Contains(const std::shared_ptr<Player>& player) const
{
    if (!player) return false;
    return Contains(player->GetAccountID());
}

void Party::AddPlayer(const std::shared_ptr<Player>& player)
{
    if (!player) return;

    // send party info to new player
    PartyJoinPacket join_packet;

    // send new player info to old members
    PartyMemberChangedPacket new_pkt;
    new_pkt.change = PartyMemberChangeType::kJoin;

    {
        std::lock_guard<std::mutex> lock(mtx_);

        auto new_char = player->GetPlayerCharacter();
        if (!new_char) return;

        players_[player->GetAccountID()] = player;

        for (auto& old_player : std::views::values(players_))
        {
            if (!old_player) continue;
            auto old_character = old_player->GetPlayerCharacter();
            if (!old_character) continue;

            PartyMemberInfo info;
            info.player_id = old_player->GetAccountID();
            info.name = old_character->GetName();
            info.lv = old_character->GetLv();
            info.hp = old_character->GetHP();
            info.max_hp = old_character->GetMaxHP();
            join_packet.members.push_back(std::move(info));
        }
        join_packet.party_name = GetPartyName();
        join_packet.party_id = GetPartyID();
        join_packet.host_id = GetHost();

        new_pkt.member.player_id = player->GetAccountID();
        new_pkt.member.name = new_char->GetName();
        new_pkt.member.lv = new_char->GetLv();
        new_pkt.member.hp = new_char->GetHP();
        new_pkt.member.max_hp = new_char->GetMaxHP();
    }

    player->SendPacket(join_packet);
    SendPacket(new_pkt, player->GetAccountID());
}

void Party::RemovePlayer(uint32_t player_id)
{
    std::shared_ptr<Player> leaving;
    bool host_changed = false;
    uint32_t new_host_id = 0;

    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = players_.find(player_id);
        if (it == players_.end())
            return;

        leaving = it->second;
        players_.erase(it);

        /*
        if (host_id_ == player_id)
        {
            if (players_.empty())
            {
                host_id_ = 0;
            }
            else
            {
                for (const auto& next_player_id : std::views::keys(players_))
                {
                    host_id_ = next_player_id;
                    
                    // 호스트 변경 알림
                }
            }
        }*/
    }

    PartyMemberChangedPacket leave_packet;
    leave_packet.change = PartyMemberChangeType::kLeave;
    leave_packet.member.player_id = player_id;
    SendPacket(leave_packet, player_id);

    if (leaving) {
        PartyLeavePacket leave;
        leaving->SendPacket(leave);
    }
}

void Party::RemovePlayer(const std::shared_ptr<Player>& player)
{
    RemovePlayer(player->GetAccountID()); 
}

int Party::GetPlayerCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return static_cast<int>(players_.size());
}

std::vector<std::shared_ptr<Player>> Party::GetPlayers() const
{
    std::vector<std::shared_ptr<Player>> snapshot;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        snapshot.reserve(players_.size());
        for (auto& [id, p] : players_) {
            if (p) snapshot.push_back(p);
        }
    }
    return snapshot;
}
