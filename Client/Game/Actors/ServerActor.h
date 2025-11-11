#pragma once
#include <CustomPacket.h>

#include "NetworkActor.h"

class ServerActor : public NetworkActor
{
    SHADER_CLASS_HELPER(ServerActor)
    GENERATED_BODY(ServerActor, NetworkActor)
    
public:
    struct MovementSnapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        
        float server_time;

        bool time_update;
    };

    struct AnimationSnapshot
    {
        bool          is_flipped;
        std::wstring  animation;
        float         server_time;
    };

    struct DamageSnapshot
    {
        int damage_amount;
        Math::Vector2 damage_effect_position;
        Math::Vector2 hit_effect_position;
        float attacker_direction;
        DamageSourceType source_type;
        uint32_t source_id;
    };
    
    ServerActor(const std::wstring& name);
    virtual ~ServerActor() override = default;

    void SetFlip(bool is_fliped);
    void PlayAnimation(const std::wstring& animation);
    virtual void TakeDamage(const std::vector<DamageInfo>& damage_amount, float server_time);
    
protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void ReceivePacket(Net::IPacket* packet) override;
    virtual void OnShowDamage(const DamageSnapshot& damage_snapshot);
    
    std::deque<MovementSnapshot>  movement_snapshots_;
    std::deque<AnimationSnapshot> animation_snapshots_;

    AnimationSnapshot prev_animation;
    
    std::deque<DamageSnapshot> pending_damages_;
    float last_damage_spawn_time_;

#pragma region 컴포넌트
    std::shared_ptr<class BoxColliderComponent> collider_;
    std::shared_ptr<class SpriteRendererComponent> renderer_;
    std::shared_ptr<class AnimatorComponent> animator_;
#pragma endregion
    
};
