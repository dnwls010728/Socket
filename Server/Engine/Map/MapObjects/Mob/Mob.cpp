#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>
#include <iostream>

#include "NetDef.h"
#include "Engine/Map/Map.h"

Mob::Mob() :
    velocity_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    gravity_(-20.f),
    is_grounded_(false)
{
    state_machine_ = std::make_unique<FSM::StateMachine>();
}

void Mob::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    state_machine_->Tick(delta_time);

    velocity_.x = -1.f;
    velocity_.y += gravity_ * delta_time;
    Math::Vector2 next_position = GetPosition() + velocity_ * delta_time;

    is_grounded_ = false;
    Foothold* foothold = map_->FindFoothold(next_position);
    if (foothold)
    {
        float y = foothold->GetYAt(next_position.x);
        if (next_position.y <= y)
        {
            next_position.y = y;
            velocity_.y = 0.f;
            is_grounded_ = true;
        }
    }

    if (next_position != last_position_)
    {
        last_position_ = next_position;
        
    }
    
    ObjectPositionPacket packet;
    packet.object_id = GetObjectID();
    packet.position_x = next_position.x;
    packet.position_y = next_position.y;
    packet.velocity_x = velocity_.x;
    packet.velocity_y = velocity_.y;
    packet.server_time = Net::GetClientTime();
    map_->SendPacket(packet);

    std::cout << "Mob position updated to: " << next_position.x << ", " << next_position.y << std::endl;
    SetPosition(next_position);
    
}
