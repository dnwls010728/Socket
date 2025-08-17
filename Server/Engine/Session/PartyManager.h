#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>
#include <string>
#include "IPacket.h"
#include "Singleton.h"

class Player;
class Party;
namespace Net { class IPacket; }

class PartyManager : public Singleton<PartyManager>
{
public:
    PartyManager();
    PartyManager(const PartyManager&)            = delete;
    PartyManager& operator=(const PartyManager&) = delete;
    PartyManager(PartyManager&&)                 = delete;
    PartyManager& operator=(PartyManager&&)      = delete;
    ~PartyManager() = default;
    
    std::shared_ptr<Party> CreateParty( const std::wstring& party_name = L"");
    void DestroyParty(std::uint32_t party_id);
    
    std::shared_ptr<Party> GetParty(std::uint32_t party_id) const;
    std::vector<std::shared_ptr<Party>> GetParties() const;
    std::vector<std::uint32_t> GetPartyIds() const;
    int GetPartyCount() const;
    
    void AddPlayerToParty(std::uint32_t party_id, const std::shared_ptr<Player>& player);
    void RemovePlayerFromParty(std::uint32_t party_id, std::uint32_t player_id);
    
    void SendPacket(std::uint32_t party_id, const Net::IPacket& packet, uint32_t exclusion_player = 0);

private:
    uint32_t AllocatePatyID();
private:
    mutable std::mutex mtx_;
    std::unordered_map<std::uint32_t, std::shared_ptr<Party>> parties_;
    std::uint32_t next_party_id_;
};