#pragma once
#include <CommonObject.h>
#include <deque>

#include "Actors/Characters/CharacterBase.h"

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)

    struct Snapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        
        bool is_flipped;
        
        std::wstring animation;
        
        float server_time;
    };
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    virtual void ReceivePacket(Net::IPacket* packet) override;

    void InitSpawn(const std::wstring& name, const Math::Vector2& position);
    
    FORCEINLINE const Math::Vector2& GetMovementInput() const { return movement_input_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;

    Math::Vector2 movement_input_;

    Math::Vector2 last_position_;
    std::deque<Snapshot> snapshots_;
    float movement_sync_accumulator_;

    int32_t timer_;
    
};
