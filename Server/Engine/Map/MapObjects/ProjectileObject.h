#pragma once

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "Engine/Map/MapObject.h"

class IDamageable;
class ProjectileData;

class ProjectileObject : public MapObject
{
public:
    ProjectileObject();
    virtual ~ProjectileObject() override = default;

    void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline void SetOwner(const std::shared_ptr<MapObject>& owner) { owner_ = owner; }
    inline std::shared_ptr<MapObject> GetOwner() const { return owner_.lock(); }

    inline void SetProjectileID(uint32_t projectile_id) { projectile_id_ = projectile_id; }
    inline uint32_t GetProjectileID() const { return projectile_id_; }

    inline void SetVelocity(const Math::Vector2& velocity) { velocity_ = velocity; }
    inline const Math::Vector2& GetVelocity() const { return velocity_; }

    inline void SetSize(const Math::Vector2& size) { size_ = size; }
    inline const Math::Vector2& GetSize() const { return size_; }

    void SetDamage(int32_t damage);
    inline int32_t GetDamage() const { return damage_; }

    void SetDamageCount(int32_t count);
    inline int32_t GetDamageCount() const { return damage_count_; }

    inline void SetMaxTargets(int32_t max_targets) { max_targets_ = max_targets; }
    inline int32_t GetMaxTargets() const { return max_targets_; }

    inline void SetMaxLifetime(float max_lifetime) { max_lifetime_ = max_lifetime; }
    inline float GetMaxLifetime() const { return max_lifetime_; }

    inline void SetAnimation(const std::wstring& animation) { animation_ = animation; }
    inline const std::wstring& GetAnimation() const { return animation_; }

    inline void SetFlip(bool is_flipped) { is_flipped_ = is_flipped; }
    inline bool IsFlipped() const { return is_flipped_; }

protected:
    void BeginPlay() override;
    void PhysicsTick(float delta_time) override;
    void Tick(float delta_time) override;

private:
    void SendPositionPacket(const Math::Vector2& position, bool time_update = false) const;
    void SendAnimationPacket(const std::wstring& animation, bool is_flip, bool instant_play = false) const;
    void ApplyDamage(const std::shared_ptr<IDamageable>& target);
    void RequestDestroy();
    void RefreshDamageAmounts();

    std::weak_ptr<MapObject> owner_;

    Math::Vector2 velocity_;
    Math::Vector2 size_;

    int32_t damage_;
    int32_t damage_count_;
    int32_t max_targets_;
    int32_t targets_hit_;

    float max_lifetime_;
    float elapsed_time_;

    uint32_t projectile_id_;

    bool is_flipped_;
    bool was_moving_;
    bool pending_destroy_;

    std::wstring animation_;
    std::wstring last_animation_;
    bool last_flipped_;

    Math::Vector2 last_position_;

    std::vector<int32_t> damage_amounts_;
    std::unordered_set<uint32_t> damaged_targets_;
};

