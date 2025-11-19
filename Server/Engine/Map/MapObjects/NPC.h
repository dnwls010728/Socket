#pragma once
#include "Map/MapObject.h"

class NPC : public MapObject
{
public:
    NPC(uint32_t npc_id);
    virtual ~NPC() override = default;
    
    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    void SendShop(const std::shared_ptr<PlayerCharacter>& player) const;

    bool HasShop() const;
    
    inline uint32_t GetNPCID() const { return npc_id_; }
    
protected:
    uint32_t npc_id_;
    
};
