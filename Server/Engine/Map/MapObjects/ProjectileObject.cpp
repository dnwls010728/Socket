#include "pch.h"
#include "ProjectileObject.h"

#include <CustomPacket.h>

#include "Engine/Map/IDamageable.h"
#include "Engine/Map/Map.h"
#include "Engine/Map/MapObject.h"
#include "Engine/Map/PlayerCharacter.h"
#include "Math/Bounds.h"
#include "Math/Math.h"
#include "NetDef.h"
#include "Misc/DelegateMacros.h"

ProjectileObject::ProjectileObject() :
    owner_(),
    velocity_(Math::Vector2::Zero()),
    size_(Math::Vector2::One()),
    damage_(0),
    damage_count_(1),
    max_targets_(0),
    targets_hit_(0),
    max_lifetime_(0.f),
    elapsed_time_(0.f),
    projectile_id_(0),
    is_flipped_(false),
    was_moving_(false),
    pending_destroy_(false),
    animation_(L"Idle"),
    last_animation_(),
    last_flipped_(false),
    last_position_(Math::Vector2::Zero()),
    damage_amounts_(),
    damaged_targets_(),
    hit_effect_pack_(),
    hit_effect_animation_(),
    owner_is_player_(true)
{
    RefreshDamageAmounts();
}

void ProjectileObject::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;

    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kProjectile;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    ProjectileInfo& info = packet.object_info.info.projectile;
    info.projectile_id = projectile_id_;
    info.owner_id = owner_.expired() ? 0 : owner_.lock()->GetObjectID();
    info.velocity_x = velocity_.x;
    info.velocity_y = velocity_.y;
    info.size_x = size_.x;
    info.size_y = size_.y;
    info.max_lifetime = max_lifetime_;
    info.is_flipped = is_flipped_;
    wcscpy_s(info.animation_name, animation_.c_str());

    player->SendPacket(packet);
}

void ProjectileObject::SetDamage(int32_t damage)
{
    damage_ = damage;
    RefreshDamageAmounts();
}

void ProjectileObject::SetDamageCount(int32_t count)
{
    damage_count_ = std::max(1, count);
    RefreshDamageAmounts();
}

void ProjectileObject::BeginPlay()
{
    MapObject::BeginPlay();

    last_position_ = position_;
    was_moving_ = false;
    elapsed_time_ = 0.f;
    targets_hit_ = 0;
    pending_destroy_ = false;
    damaged_targets_.clear();
    last_animation_.clear();
    last_flipped_ = !is_flipped_;
}

void ProjectileObject::PhysicsTick(float delta_time)
{
    MapObject::PhysicsTick(delta_time);

    if (pending_destroy_) return;

    if (!Math::IsEqual(velocity_.x, 0.f) || !Math::IsEqual(velocity_.y, 0.f))
    {
        Translate(velocity_ * delta_time);
    }

    if (!map_) return;

    if (max_lifetime_ <= 0.f)
    {
        const Bounds& map_bounds = map_->GetMapBounds();
        Bounds projectile_bounds(position_, size_);
        if (!Bounds::Contains(map_bounds, projectile_bounds))
        {
            RequestDestroy();
        }
    }
}

void ProjectileObject::Tick(float delta_time)
{
    MapObject::Tick(delta_time);

    if (pending_destroy_) return;

    elapsed_time_ += delta_time;
    if (max_lifetime_ > 0.f && elapsed_time_ >= max_lifetime_)
    {
        RequestDestroy();
        return;
    }

    if (!map_) return;

    Bounds search_bounds(position_, size_);
    std::vector<std::shared_ptr<MapObject>> overlap_objects;
    map_->GetOverlappingObjects(search_bounds, overlap_objects);

    auto owner_player = owner_.lock();
    uint32_t owner_id = owner_player ? owner_player->GetObjectID() : 0;
    int owner_party_id = 0;
    if (owner_is_player_)
    {
        PlayerCharacter* owner_character = static_cast<PlayerCharacter*>(owner_player.get());
        owner_party_id = owner_character->GetPartyID();
    }
    
    auto cmp = [&](std::shared_ptr<MapObject> left, std::shared_ptr<MapObject> right)
    {
        Math::Vector2 left_pos = left->GetPosition();
        Math::Vector2 right_pos = right->GetPosition();
        float left_dist = Math::Vector2::Distance(position_, left_pos);
        float right_dist = Math::Vector2::Distance(position_, right_pos);
        return left_dist > right_dist;
    };
    std::priority_queue<std::shared_ptr<MapObject>, std::vector<std::shared_ptr<MapObject>>, decltype(cmp)> pq(cmp);
    
    for (const auto& object : overlap_objects)
    {
        if (!object) continue;
        uint32_t object_id = object->GetObjectID();
        if (object_id == object_id_) continue;
        if (object_id == owner_id) continue;
        if (damaged_targets_.contains(object_id)) continue;

        if (owner_party_id != 0)
        {
            auto target_player = std::dynamic_pointer_cast<PlayerCharacter>(object);
            if (target_player && target_player->GetPartyID() == owner_party_id)
                continue;
        }

        auto damageable = std::dynamic_pointer_cast<IDamageable>(object);
        if (!damageable) continue;

        pq.push(object);
    }

    while (!pq.empty())
    {
        auto object = pq.top();
        pq.pop();
        
        auto damageable = std::dynamic_pointer_cast<IDamageable>(object);
        if (!damageable) continue;

        ApplyDamage(damageable);
        damaged_targets_.insert(object->GetObjectID());
        ++targets_hit_;

        if (max_targets_ > 0 && targets_hit_ >= max_targets_)
        {
            RequestDestroy();
            break;
        }
    }

    if (pending_destroy_) return;

    Math::Vector2 position = GetPosition();
    if (position != last_position_)
    {
        if (!was_moving_) SendPositionPacket(last_position_, true);
        SendPositionPacket(position);
        was_moving_ = true;
        last_position_ = position;
    }
    else if (was_moving_)
    {
        SendPositionPacket(position);
        was_moving_ = false;
    }

    if (animation_ != last_animation_ || is_flipped_ != last_flipped_)
    {
        SendAnimationPacket(animation_, is_flipped_);
        last_animation_ = animation_;
        last_flipped_ = is_flipped_;
    }
}

void ProjectileObject::SendPositionPacket(const Math::Vector2& position, bool time_update) const
{
    if (!map_) return;

    ObjectPositionPacket packet;
    packet.object_id = object_id_;
    packet.position_x = position.x;
    packet.position_y = position.y;
    packet.velocity_x = velocity_.x;
    packet.velocity_y = velocity_.y;
    packet.server_time = Net::GetClientTime();
    packet.time_update = time_update;
    map_->SendPacket(packet);
}

void ProjectileObject::SendAnimationPacket(const std::wstring& animation, bool is_flip, bool instant_play) const
{
    if (!map_) return;

    ObjectAnimationPacket packet;
    packet.object_id = object_id_;
    packet.animation = animation;
    packet.is_flipped = is_flip;
    packet.server_time = Net::GetClientTime();
    packet.instant_play = instant_play;
    map_->SendPacket(packet);
}

void ProjectileObject::ApplyDamage(const std::shared_ptr<IDamageable>& target)
{
    if (!target) return;

    uint32_t owner_id = owner_.expired() ? object_id_ : owner_.lock()->GetObjectID();

    Bounds projectile_bounds(position_, size_);
    Bounds target_bounds = target->GetBounds();
    Bounds intersect_bounds = Bounds::Intersect(projectile_bounds, target_bounds);

    auto generate_position = [&](const Bounds& fallback) -> Math::Vector2
    {
        const Bounds& spawn_bounds = (intersect_bounds.size.x > 0.f && intersect_bounds.size.y > 0.f) ? intersect_bounds : fallback;
        float x = Math::RandRange(spawn_bounds.min.x, spawn_bounds.max.x);
        float y = Math::RandRange(spawn_bounds.min.y, spawn_bounds.max.y);
        return {x, y};
    };

    std::vector<DamageHitInfo> damage_infos;
    if (damage_amounts_.empty())
    {
        DamageHitInfo info;
        info.damage_amount = damage_;
        info.position = generate_position(target_bounds);
        info.effect_pack = hit_effect_pack_;
        info.effect_animation = hit_effect_animation_;
        damage_infos.push_back(info);
    }
    else
    {
        damage_infos.reserve(damage_amounts_.size());
        for (int32_t damage_value : damage_amounts_)
        {
            DamageHitInfo info;
            info.damage_amount = damage_value;
            info.position = generate_position(target_bounds);
            info.effect_pack = hit_effect_pack_;
            info.effect_animation = hit_effect_animation_;
            damage_infos.push_back(info);
        }
    }

    target->TakeMultiDamage(owner_id, damage_infos);
}

void ProjectileObject::RequestDestroy()
{
    if (pending_destroy_) return;
    pending_destroy_ = true;

    if (!map_) return;

    map_->DestroyProjectile(object_id_);
}

void ProjectileObject::RefreshDamageAmounts()
{
    damage_amounts_.assign(std::max(1, damage_count_), damage_);
}

