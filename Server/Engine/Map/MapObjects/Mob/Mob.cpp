#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>

#include "NetDef.h"
#include "Engine/Map/Map.h"
#include "Math/Math.h"
#include "Session/Player.h"

Mob::Mob() :
    velocity_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    gravity_(-20.f),
    timer_(0.f),
    direction_(0),
    is_grounded_(false),
    foothold_(nullptr),
    hp_(3000)
{
    state_machine_ = std::make_unique<FSM::StateMachine>();
    direction_ = Math::RandRange(-1, 1);
}

void Mob::PhysicsTick(float delta_time)
{
    MapObject::PhysicsTick(delta_time);

    velocity_.x = direction_ * 2.f;
    velocity_.y += gravity_ * delta_time;
    Math::Vector2 next_position = GetPosition() + velocity_ * delta_time;

    is_grounded_ = false;
    foothold_ = map_->FindFoothold({ next_position.x, GetPosition().y + .1f }); // 경사면 체크를 위해 y 좌표를 0.1만큼 올림
    if (foothold_)
    {
        float foothold_y = foothold_->GetYAt(next_position.x);
        if (GetPosition().y + 1.f >= foothold_y && next_position.y <= foothold_y)
        {
            next_position.y = foothold_y;
            velocity_.y = 0.f;
            is_grounded_ = true;
        }
    }
    
    SetPosition(next_position);
}

void Mob::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    state_machine_->Tick(delta_time);
    
    timer_ += delta_time;
    if (timer_ >= 1.6f)
    {
        timer_ -= 1.6f;
        direction_ = Math::RandRange(-1, 1);
    }

    Math::Vector2 next_position = GetPosition();
    
    if (next_position != last_position_)
    {
        last_position_ = next_position;
        
        ObjectPositionPacket packet;
        packet.object_id = GetObjectID();
        packet.position_x = next_position.x;
        packet.position_y = next_position.y;
        packet.velocity_x = velocity_.x;
        packet.velocity_y = velocity_.y;
        packet.server_time = Net::GetClientTime();
        map_->SendPacket(packet);
    }
    
    SetPosition(next_position);
    
}

void Mob::OnHit(int32_t damage)
{
    if (hp_ <= 0) return;
    
    hp_ -= damage;
    if (hp_ <= 0)
    {
        hp_ = 0;
        map_->DestroyObject(GetObjectID());
    }
}
