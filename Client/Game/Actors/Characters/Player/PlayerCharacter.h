#pragma once
#include <CommonObject.h>

#include "Actors/Characters/CharacterBase.h"
class InventoryComponent;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    virtual void ReceivePacket(Net::IPacket* packet) override;

    void InitSpawn(const Math::Vector2& position);

    FORCEINLINE Math::Vector2& GetVelocity() { return velocity_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;

#pragma region 컴포넌트
    std::shared_ptr<InventoryComponent> inventory_;
#pragma endregion

    Math::Vector2 movement_input_;

    Movement last_movement_;
    std::queue<Movement> movements_;

    bool is_jump_;

    int timer_;
    
};
