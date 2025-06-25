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
    hp_(3000),
    is_flipped_(false),
    animation_(L"Idle"),
    prev_is_moving_(false)
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

    // 테스트
    if (direction_ > 0)
        is_flipped_ = false;
    else if (direction_ < 0)
        is_flipped_ = true;

    if (!Math::IsEqual(velocity_.x, 0.f)) animation_ = L"Walk";
    else animation_ = L"Idle";
    
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
        if (prev_is_moving_ == false)
        {
            ObjectPositionPacket dummy_packet;
            dummy_packet.object_id = GetObjectID();
            dummy_packet.position_x = last_position_.x;
            dummy_packet.position_y = last_position_.y;
            dummy_packet.velocity_x = velocity_.x;
            dummy_packet.velocity_y = velocity_.y;
            dummy_packet.is_flipped = is_flipped_;
            dummy_packet.animation = animation_;
            dummy_packet.server_time = Net::GetClientTime();
            dummy_packet.time_update = true;
            map_->SendPacket(dummy_packet);
        }

        ObjectPositionPacket packet;
        packet.object_id = GetObjectID();
        packet.position_x = next_position.x;
        packet.position_y = next_position.y;
        packet.velocity_x = velocity_.x;
        packet.velocity_y = velocity_.y;
        packet.is_flipped = is_flipped_;
        packet.animation = animation_;
        packet.server_time = Net::GetClientTime();
        packet.time_update = false;
        map_->SendPacket(packet);

        prev_is_moving_ = true;
        last_position_ = next_position;
    }
    else
    {
        if (prev_is_moving_)
        {
            Math::Vector2 stop_position = GetPosition();
            ObjectPositionPacket stop_packet;
            stop_packet.object_id = GetObjectID();
            stop_packet.position_x = stop_position.x;
            stop_packet.position_y = stop_position.y;
            stop_packet.velocity_x = velocity_.x;
            stop_packet.velocity_y = velocity_.y;
            stop_packet.is_flipped = is_flipped_;
            stop_packet.animation = animation_;
            stop_packet.server_time = Net::GetClientTime();
            stop_packet.time_update = false;
            map_->SendPacket(stop_packet);
        }
        prev_is_moving_ = false;
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
