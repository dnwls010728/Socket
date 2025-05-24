#pragma once
#include "Math/Vector2.h"
#include "tmxlite/Types.hpp"

class MapObject
{
public:
    MapObject();
    virtual ~MapObject() = default;

    inline void SetObjectID(uint32_t id) { object_id_ = id; }
    inline uint32_t GetObjectID() const { return object_id_; }

    inline void SetPosition(const Math::Vector2& position) { position_ = position; }
    inline const Math::Vector2& GetPosition() const { return position_; }

private:
    uint32_t object_id_;

    Math::Vector2 position_;
    
};
