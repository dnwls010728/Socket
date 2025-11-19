#pragma once
#include "Engine/Map/MapObject.h"

class Item;

class DroppedItem : public MapObject
{
public:
    DroppedItem();
    virtual ~DroppedItem() override = default;

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline void SetDropper(const std::shared_ptr<MapObject>& dropper) { dropper_ = dropper; }
    inline std::shared_ptr<MapObject> GetDropper() const { return dropper_.lock(); }

    inline void SetItem(const std::shared_ptr<Item>& item) { item_ = item; }
    inline std::shared_ptr<Item> GetItem() const { return item_; }

    inline void SetColor(int32_t color) { color_ = color; }
    inline int32_t GetColor() const { return color_; }

    inline bool IsColor() const { return color_ > 0; }
    
    inline void SetDroppedTime(double time) { dropped_time_ = time; }
    inline double GetDroppedTime() const { return dropped_time_; }
    
protected:
    virtual void Tick(float delta_time) override;

private:
    std::weak_ptr<MapObject> dropper_;

    std::shared_ptr<Item> item_;

    int32_t color_;
    
    double dropped_time_;
    
};
