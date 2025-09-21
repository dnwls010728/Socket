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
            map->SendPacket(packet);
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

    auto attack_chain = std::make_shared<AttackChain>(frame);
    
    uint32_t hit_count = 0;
    for (const auto& target : target_objs)
    {
        if (!target) continue;

        if (target.get() == owner_) continue;

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