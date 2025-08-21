#pragma once
#include <cstdint>

#include "Math/Vector2.h"

class Portal
{
public:
    Portal(int32_t id, int32_t to_id, int32_t to_map, const Math::Vector2& position);
    ~Portal() = default;

    inline void SetID(int32_t id) { id_ = id; }
    inline int32_t GetID() const { return id_; }

    inline int32_t GetToID() const { return to_id_; }
    inline void SetToID(int32_t to_id) { to_id_ = to_id; }

    inline void SetToMap(int32_t to_map) { to_map_ = to_map; }
    inline int32_t GetToMap() const { return to_map_; }

    inline void SetPosition(const Math::Vector2& position) { position_ = position; }
    inline const Math::Vector2& GetPosition() const { return position_; }

private:
    int32_t id_;
    int32_t to_id_;
    int32_t to_map_;

    Math::Vector2 position_;
    
};
