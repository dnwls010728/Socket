#include "pch.h"
#include "Mob.h"

#include "Engine/Map/Map.h"

Mob::Mob() :
    velocity_(Math::Vector2::Zero()),
    gravity_(-20.f)
{
}

void Mob::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    velocity_.y += gravity_ * delta_time;
    Math::Vector2 next_position = position_ + velocity_ * delta_time;

    Foothold foothold = map_->GetFoothold(next_position);
    if (foothold.IsValid())
    {
        float foothold_y = map_->GetFootholdY(foothold, next_position);
        if (next_position.y < foothold_y)
        {
            next_position.y = foothold_y;
            velocity_.y = 0.f;
        }
    }

    position_ = next_position;
}
