#pragma once
#include "Math/Vector2.h"
#include "tmxlite/Types.hpp"

class Map;

class MapObject : public std::enable_shared_from_this<MapObject>
{
public:
    MapObject();
    virtual ~MapObject() = default;

    inline void SetObjectID(uint32_t id) { object_id_ = id; }
    inline uint32_t GetObjectID() const { return object_id_; }

    inline void SetMap(Map* map) { map_ = map; }
    inline Map* GetMap() const { return map_; }

    inline void SetPosition(const Math::Vector2& position) { position_ = position; }
    inline const Math::Vector2& GetPosition() const { return position_; }

    inline void Translate(const Math::Vector2& translation) { position_ += translation; }

protected:
    friend class Map;

    inline virtual void BeginPlay() {}
    inline virtual void PhysicsTick(float delta_time) {}
    inline virtual void Tick(float delta_time) {}
    
    uint32_t object_id_;

    Map* map_;

    Math::Vector2 position_;
    
};
