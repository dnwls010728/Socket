#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>

#include "NetDef.h"
#include "Engine/Map/Map.h"
#include "Session/Player.h"
#include "States/MobHitState.h"
#include "States/MobIdleState.h"
#include "States/MobWalkState.h"

Mob::Mob() :
    velocity_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    gravity_(-20.f),
    is_grounded_(false),
    prev_is_moving_(false),
    is_flipped_(false),
    foothold_(nullptr),
    hp_(3000),
    animation_(L"Idle")
{
    state_machine_ = std::make_unique<FSM::StateMachine>();
    
}

void Mob::BeginPlay()
{
    MapObject::BeginPlay();

    std::shared_ptr<Mob> shared_ptr = std::static_pointer_cast<Mob>(shared_from_this());
    idle_state_ = std::make_shared<MobIdleState>(shared_ptr, *state_machine_);
    walk_state_ = std::make_shared<MobWalkState>(shared_ptr, *state_machine_);
    hit_state_ = std::make_shared<MobHitState>(shared_ptr, *state_machine_);
    
    state_machine_->GetOrAddNode(idle_state_);
    state_machine_->GetOrAddNode(walk_state_);
    state_machine_->GetOrAddNode(hit_state_);
    
    state_machine_->SetState(idle_state_);
    
}

void Mob::PhysicsTick(float delta_time)
{
    MapObject::PhysicsTick(delta_time);
    state_machine_->PhysicsTick(delta_time);

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

    Math::Vector2 position = GetPosition();
    
    if (position != last_position_)
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
        packet.position_x = position.x;
        packet.position_y = position.y;
        packet.velocity_x = velocity_.x;
        packet.velocity_y = velocity_.y;
        packet.is_flipped = is_flipped_;
        packet.animation = animation_;
        packet.server_time = Net::GetClientTime();
        packet.time_update = false;
        map_->SendPacket(packet);

        prev_is_moving_ = true;
        last_position_ = position;
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
    
}

void Mob::OnHit(int32_t damage)
{
    if (hp_ <= 0) return;

    state_machine_->ChangeState(hit_state_);
    
    hp_ -= damage;
    if (hp_ <= 0)
    {
        hp_ = 0;
        map_->DestroyObject(GetObjectID());
    }
}
