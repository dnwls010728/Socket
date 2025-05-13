#pragma once
#include "Actors/Characters/CharacterBase.h"
 
class MonsterBase : public CharacterBase
{
    SHADER_CLASS_HELPER(MonsterBase)
    GENERATED_BODY(MonsterBase, CharacterBase)
 
public:
    enum class State : uint8_t
    {
        kIdle,
        kMove
    };
     
    MonsterBase(const std::wstring& kName);
    virtual ~MonsterBase() override = default;
 
    virtual void ReceivePacket(Net::IPacket* packet) override;
 
protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
 
private:
    Math::Vector2 velocity_;
 
    State state_;
 
    int counter_;
 
};