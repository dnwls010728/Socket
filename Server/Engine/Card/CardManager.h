#pragma once
#include <array>
#include <CommonObject.h>
#include <vector>
#include <mutex>
#include <queue>
#include <unordered_map>

class CardData;
class PlayerCharacter;
namespace sql { class Connection; }

class CardManager
{
public:
    using CardGroup = std::array<CardSelectInfo, 3>;
    
    CardManager(PlayerCharacter* player);
    ~CardManager() = default;
    
    virtual void OnLevelUp();
    virtual void OnCardSelected(const CardSelectInfo& card);
    virtual void OnLoadCharacter();
    virtual void OnEnterMap();
    virtual void OnUpdateDatabase();

    int GetMaxHP() const { return effective_max_hp_; }
    int GetATK()   const { return effective_atk_; }
    int GetDEF()   const { return effective_def_; }
    int GetDIG()   const { return effective_dig_; }

private:
    const CardData* GetRandomCardData() const;
    bool CreatePendingCard(CardGroup& cards) const;
    bool ActivatePendingCard(CardGroup& group) const;
    bool CreateCards(CardGroup& cards) const;
    void SendSelectCardPacket(const CardGroup& cards) const;
    bool CreateUUID(std::string& uuid) const;
    std::string NowForDB() const;
    void ComputeStats();

    void LoadOwnedCards(sql::Connection* connection);
    void LoadOffers(sql::Connection* connection);

private:
    std::mutex mtx_;
    std::vector<CardSelectInfo> selecting_cards;
    std::queue<CardGroup> pending_cards_;
    PlayerCharacter* player_;

    std::unordered_map<uint32_t, int> owned_cards_;

    int effective_max_hp_ = 0;
    int effective_atk_    = 0;
    int effective_def_    = 0;
    int effective_dig_    = 0;
};
