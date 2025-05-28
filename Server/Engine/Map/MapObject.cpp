#include "pch.h"
#include "MapObject.h"

#include <CustomPacket.h>
#include <iostream>

#include "Map.h"

MapObject::MapObject() :
    object_id_(0),
    map_(nullptr),
    position_(Math::Vector2::Zero())
{
}

void MapObject::Tick(float delta_time)
{
    // 테스트 코드
    static Math::Vector2 velocity = Math::Vector2::Zero();

    velocity.x = -1.f;
    velocity.y += -20.f * delta_time;

    Math::Vector2 next = position_ + velocity * delta_time;

    Foothold foothold = map_->GetFoothold(position_ + Math::Vector2::Up() *.1f);
    if (foothold.IsValid())
    {
        float y = map_->GetFootholdY(foothold, position_ + Math::Vector2::Up() *.1f);
        if (next.y < y)
        {
            position_.y = y;
            velocity.y = 0.f;
        }
        else position_.y = next.y;
    }
    else position_.y = next.y;

    position_.x = next.x;

    MoveTestPacket packet;
    packet.position_x = position_.x;
    packet.position_y = position_.y;
    map_->SendPacket(packet);

    std::cout << "Position: " << position_.x << ", " << position_.y << std::endl;
}
