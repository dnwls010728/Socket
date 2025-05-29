#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>

#include "NetDef.h"
#include "Engine/Map/Map.h"
#include "Session/Player.h"

Mob::Mob() :
    velocity_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    gravity_(-20.f)
{
}

void Mob::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    const auto& players = map_->GetPlayers();
    if (players.empty()) return;

    velocity_.x = players[0].lock()->GetPosition().x - position_.x;
    velocity_.y += gravity_ * delta_time;
    Math::Vector2 next_position = position_ + velocity_ * delta_time;

    Foothold foothold = map_->GetFoothold(next_position + Math::Vector2::Up());
    if (foothold.IsValid())
    {
        float foothold_y = map_->GetFootholdY(foothold, next_position + Math::Vector2::Up());
        if (next_position.y < foothold_y)
        {
            next_position.y = foothold_y;
            velocity_.y = 0.f;
        }
    }

    position_ = next_position;
    if (position_ != last_position_)
    {
        last_position_ = position_;

        MoveTestPacket pkt;
        pkt.object_id = object_id_;
        pkt.position_x = position_.x;
        pkt.position_y = position_.y;
        pkt.velocity_x = velocity_.x;
        pkt.velocity_y = velocity_.y;
        pkt.server_time = Net::GetClientTime();
        map_->SendPacket(pkt);
    }
}
