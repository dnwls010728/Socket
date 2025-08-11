#pragma once
#include "Engine/Map/MapObject.h"

class DroppedItem : public MapObject
{
public:
    DroppedItem();
    virtual ~DroppedItem() override = default;

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline void SetDropper(const std::shared_ptr<MapObject>& dropper) { dropper_ = dropper; }
    inline std::shared_ptr<MapObject> GetDropper() const { return dropper_.lock(); }

    inline void SetItemID(uint32_t item_id) { item_id_ = item_id; }
    inline uint32_t GetItemID() const { return item_id_; }

    inline void SetCount(int32_t count) { count_ = count; }
    inline int32_t GetCount() const { return count_; }

private:
    std::weak_ptr<MapObject> dropper_;
    
    uint32_t item_id_;
    int32_t count_;
    
};
