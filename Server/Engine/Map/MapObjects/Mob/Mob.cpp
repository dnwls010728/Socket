#include "pch.h"
#include "Mob.h"

#include <CustomPacket.h>
#include <iostream>

#include "DataManager.h"
#include "NetDef.h"
#include "Engine/Map/Map.h"
#include "Map/PlayerCharacter.h"
#include "Math/Math.h"
#include "Session/Player.h"
#include "States/MobHitState.h"
#include "States/MobIdleState.h"
#include "States/MobWalkState.h"

Mob::Mob(const MobData& mob_data) :
    mob_id_(mob_data.mob_id),
    damage_(mob_data.stats.dmg),
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
    
}

void Mob::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;

    SpawnObjectPacket packet;
    packet.object_info.type = ObjectType::kMob;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    MobInfo& info = packet.object_info.info.mob;
    info.mob_id = mob_id_;
    
    player->SendPacket(packet);
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

    foothold_ = map_->FindFoothold(position_);
    
}

void Mob::PhysicsTick(float delta_time)
{
    MapObject::PhysicsTick(delta_time);

    bool was_grounded = is_grounded_;
    state_machine_->PhysicsTick(delta_time);

    velocity_.y += gravity_ * delta_time;

    if (was_grounded && velocity_.y > 0.f)
    {
        is_grounded_ = false;
        foothold_ = nullptr;
    }
    
    Math::Vector2 next_position = position_ + velocity_ * delta_time;

    const Bounds& bounds = map_->GetMapBounds();
    next_position.x = Math::Clamp(next_position.x, bounds.min.x, bounds.max.x);

    if (foothold_)
    {
        if (next_position.x < foothold_->GetX1())
            foothold_ = map_->FindFootholdByID(foothold_->GetPreviousID());
        else if (next_position.x > foothold_->GetX2())
            foothold_ = map_->FindFootholdByID(foothold_->GetNextID());
    }

    if (velocity_.y < 0.f && !foothold_)
        foothold_ = map_->FindFoothold(next_position);

    if (foothold_)
    {
        float ground_y = foothold_->GetYAt(next_position.x);
        if (next_position.y < ground_y)
        {
            next_position.y = ground_y;
            velocity_.y = 0.0f;
            is_grounded_ = true;
        }
        else is_grounded_ = false;
    }
    else is_grounded_ = false;
    
    next_position.y = Math::Clamp(next_position.y, bounds.min.y, bounds.max.y);
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
        SendAnimationPacket(animation_, is_flipped_);
            
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

void Mob::SendAnimationPacket(const std::wstring& animation, bool is_flip, bool instant_play) const
{
    ObjectAnimationPacket packet;
    packet.object_id = GetObjectID();
    packet.animation = animation;
    packet.is_flipped = is_flip;
    packet.server_time = Net::GetClientTime();
    packet.instant_play = instant_play;
    map_->SendPacket(packet);
}

void Mob::OnHit(uint32_t attacker, uint32_t damage)
{
    if (hp_ <= 0) return;

    const auto& player = map_->FindPlayer(attacker);
    state_machine_->ChangeState(hit_state_);
    
    last_animation_ = animation_;
    hp_ -= damage;
     if (hp_ <= 0)
     {
         SendAnimationPacket(L"Die", is_flipped_, true);
         if (player) player->GainExp(10000); // 예시로 100 경험치 추가
         hp_ = 0;
         map_->DestroyObject(GetObjectID());
     }
     else
    {
        SendAnimationPacket(L"Hit", is_flipped_, true);
    }
}
