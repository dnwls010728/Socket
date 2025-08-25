#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstdint>
#include "IPacket.h"
#include <CommonObject.h>

class Player;

class Party
{
    friend class PartyManager;
public:

    Party(uint32_t party_id);
    Party()                        = delete;
    Party(const Party&)            = delete;
    Party& operator=(const Party&) = delete;
    Party(Party&&)                 = delete;
    Party& operator=(Party&&)      = delete;
    ~Party() = default;
    
    void SendPacket(const Net::IPacket& packet, uint32_t exclusion_member = 0);

    bool Contains(uint32_t character_id) const;
    bool Contains(const std::shared_ptr<Player>& player) const;

    inline void SetPartyName(const std::wstring& party_name) {party_name_ = party_name;}
    inline std::wstring GetPartyName() const { return party_name_; }

    inline void SetPartyID(std::uint32_t party_id) {party_id_ = party_id; }
    inline std::uint32_t GetPartyID() const { return party_id_; }
    
    inline void SetHost(uint32_t character_id) {  host_id_ = character_id; }
    inline uint32_t GetHost() const { return host_id_; }

    int GetPlayerCount() const;
    std::vector<std::shared_ptr<Player>> GetPlayers() const;

    void DelegateHost(uint32_t new_host_id);
    void DeleteMember(uint32_t character_id);

private:
    void AddPlayer(const std::shared_ptr<Player>& player);
    void AddOfflineMember(const PartyMemberInfo& info);
    void RemovePlayer(uint32_t character_id);
    void RemovePlayer(const std::shared_ptr<Player>& player);
private:
    mutable std::mutex mtx_;
    
    std::wstring  party_name_;
    std::uint32_t party_id_;
    
    struct Member
    {
        PartyMemberInfo info;
        std::shared_ptr<Player> player;
    };

    std::unordered_map<uint32_t, Member> members_;
    uint32_t host_id_;
};
