#pragma once
#include <cstdint>

class Actor
{
public:
    enum class Type
    {
        kPlayer,
        KMob,
        kNPC
    };
    
    Actor();
    virtual ~Actor() = default;

    virtual Type GetActorType() const = 0;

    inline uint32_t GetActorID() const { return actor_id_; }
    inline void SetActorID(uint32_t id) { actor_id_ = id; }

    inline float GetPositionX() const { return position_x_; }
    inline void SetPositionX(float x) { position_x_ = x; }

    inline float GetPositionY() const { return position_y_; }
    inline void SetPositionY(float y) { position_y_ = y; }

private:
    uint32_t actor_id_;

    float position_x_;
    float position_y_;
    
};
