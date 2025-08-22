#pragma once
#include <cstdint>
#include <memory>

#include "Math/Vector2.h"

class SpawnPoint : public std::enable_shared_from_this<SpawnPoint>
{
public:
    SpawnPoint(uint32_t mob_id, const Math::Vector2& position);
    ~SpawnPoint() = default;

    inline uint32_t GetMobID() const { return mob_id_; }
    inline const Math::Vector2& GetPosition() const { return position_; }

private:
    uint32_t mob_id_;

    Math::Vector2 position_;
    
};
