#pragma once
#include <cstdint>

#include "Math/Vector2.h"

class MapObject
{
public:
    MapObject();
    virtual ~MapObject() = default;
    
    inline void SetObjectId(uint32_t object_id) { object_id_ = object_id; }
    inline uint32_t GetObjectId() const { return object_id_; }

    inline void SetPosition(const Math::Vector2& position) { position_ = position; }
    inline const Math::Vector2& GetPosition() const { return position_; }

private:
    uint32_t object_id_;

    Math::Vector2 position_;
    
};
