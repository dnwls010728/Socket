#include "pch.h"
#include "ActiveSkill.h"

#include <CustomPacket.h>
#include <iostream>

#include "Map/PlayerCharacter.h"
#include "Map/Map.h"
#include "Map/MapObject.h"
#include "Map/IDamageable.h"
#include "Skill/AttackChain.h"
#include "Skill/AttackEffect/DamageEffect.h"
#include "Map/MapObjects/ProjectileObject.h"

ActiveSkill::ActiveSkill(PlayerCharacter* owner,  const SkillData* data, int32_t level)
: Skill(owner, data, false, level), next_frame_(0), elapsed_(0.f), is_flipped_(false), owner_position_(Math::Vector2::Zero())
{
}

void ActiveSkill::OnStart()
{
    next_frame_ = 0;
    elapsed_ = 0.f;

    if (owner_)
    {
        owner_position_ = owner_->GetPosition();
        is_flipped_ = owner_->IsFlipped();

        Map* map = owner_->GetMap();
        if (map)
        {
            SkillCastPacket packet;
            packet.skill_id = data_->id;
            packet.owner_id = owner_->GetObjectID();
            packet.cooldown_expired_time = GetCoolDownExpireTime();
            map->SendPacket(packet);

            if (data_->projectile_id != 0)
            {
                const ProjectileData* projectile_data = DataManager::Get()->GetProjectile(data_->projectile_id);
                if (projectile_data)
                {
                    std::shared_ptr<ProjectileObject> projectile = std::make_shared<ProjectileObject>();
                    projectile->SetProjectileID(projectile_data->id);

                    if (auto owner_shared = owner_->shared_from_this())
                        projectile->SetOwner(owner_shared);

                    projectile->SetAnimation(projectile_data->animation);
                    projectile->SetFlip(is_flipped_);
                    projectile->SetSize(projectile_data->size);
                    projectile->SetDamage(projectile_data->damage);
                    projectile->SetDamageCount(projectile_data->damage_count);
                    projectile->SetMaxTargets(projectile_data->max_targets);
                    projectile->SetMaxLifetime(projectile_data->max_lifetime);

                    Math::Vector2 direction = is_flipped_ ? Math::Vector2::Left() : Math::Vector2::Right();
                    projectile->SetVelocity(direction * projectile_data->speed);

                    Math::Vector2 spawn_offset = direction * (projectile_data->size.x * 0.5f + 0.5f);
                    spawn_offset.y += 0.5f;
                    projectile->SetPosition(owner_position_ + spawn_offset);

                    map->SpawnProjectile(projectile);
                }

                Stop();
                return;
            }
        }
    }
}

void ActiveSkill::Tick(float delta_time)
{
    if (!IsStart()) return;
    if (next_frame_ >= data_->hit_frames.size()) return;
    if (owner_ == nullptr) return;

    elapsed_ += delta_time;
    while (next_frame_ < data_->hit_frames.size() &&
           elapsed_ >= data_->hit_frames[next_frame_].time_offset)
    {
        ApplyHitFrame(data_->hit_frames[next_frame_]);
        ++next_frame_;
    }
    if (next_frame_ >= data_->hit_frames.size())
    {
        Stop();
    }
}

void ActiveSkill::ApplyHitFrame(const HitFrame& frame)
{
    if (owner_ == nullptr) return;
    
    Map* map = owner_->GetMap();
    if (map == nullptr) return;

    // Attach
    Bounds hitbox = {
        owner_position_ + frame.hitbox.center * (is_flipped_ ? -1.f : 1.f),
        frame.hitbox.size
    };

    std::vector<std::shared_ptr<MapObject>> target_objs;
    map->GetOverlappingObjects(hitbox, target_objs);

    const uint32_t owner_party_id = owner_->GetPartyID();

    auto attack_chain = std::make_shared<AttackChain>(frame);
    
    uint32_t hit_count = 0;
    for (const auto& target : target_objs)
    {
        if (!target) continue;

        if (target.get() == owner_) continue;

        if (owner_party_id != 0)
        {
            auto target_player = std::dynamic_pointer_cast<PlayerCharacter>(target);
            if (target_player && target_player->GetPartyID() == owner_party_id)
                continue;
        }

        auto damageable = std::dynamic_pointer_cast<IDamageable>(target);
        if (!damageable) continue;

        AttackEffect::AttackContext ctx;
        ctx.attacker = owner_;
        ctx.target = damageable.get();
        attack_chain->Apply(ctx);
        if (++hit_count >= static_cast<uint32_t>(frame.max_targets))
            break;
    }
}