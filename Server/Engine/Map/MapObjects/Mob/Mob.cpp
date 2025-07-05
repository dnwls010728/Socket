#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>
#include <iostream>

#include "DataManager.h"
#include "NetDef.h"
#include "Engine/Map/Map.h"
#include "Session/Player.h"
#include "States/MobHitState.h"
#include "States/MobIdleState.h"
#include "States/MobWalkState.h"

Mob::Mob(const MobData& mob_data) :
    mob_id_(mob_data.mob_id),
    velocity_(Math::Vector2::Zero()),
    last_position_(Math::Vector2::Zero()),
    gravity_(-20.f),
    move_speed_(mob_data.stats.speed),
    is_grounded_(false),
    was_moving_(false),
    foothold_(nullptr),
    hp_(mob_data.stats.hp),
    animation_(L"Idle"),
    is_flipped_(false),
    last_flipped_(false)
{
    state_machine_ = std::make_unique<FSM::StateMachine>();

    std::wcout << mob_data.stats.lv << L" " 
              << mob_data.stats.hp << L" "
              << mob_data.stats.speed << L" "
              << mob_data.animation_pack << std::endl;
    
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
        if (was_moving_ == false) SendPositionPacket(last_position_, true);
        SendPositionPacket(position);

        was_moving_ = true;
        last_position_ = position;
    }
    else
    {
        if (was_moving_)
        {
            Math::Vector2 stop_position = GetPosition();
            SendPositionPacket(stop_position);
        }
        
        was_moving_ = false;
    }
    
    if (animation_ != last_animation_ || is_flipped_ != last_flipped_)
    {
        SendAnimationPoacket(animation_, is_flipped_);
            
        last_animation_ = animation_;
        last_flipped_ = is_flipped_;
    }
    
}

void Mob::SendPositionPacket(const Math::Vector2& position, bool time_update) const
{
    ObjectPositionPacket packet;
    packet.object_id = GetObjectID();
    packet.position_x = position.x;
    packet.position_y = position.y;
    packet.velocity_x = velocity_.x;
    packet.velocity_y = velocity_.y;
    packet.server_time = Net::GetClientTime();
    packet.time_update = time_update;
    map_->SendPacket(packet);
}

void Mob::SendAnimationPoacket(const std::wstring& animation, bool is_flip) const
{
    ObjectAnimationPacket packet;
    packet.object_id = GetObjectID();
    packet.animation = animation;
    packet.is_flipped = is_flip;
    packet.server_time = Net::GetClientTime();
    map_->SendPacket(packet);
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
