#pragma once
#include "Actors/Characters/ServerObject.h"
 
class MonsterBase : public ServerObject
{
    SHADER_CLASS_HELPER(MonsterBase)
    GENERATED_BODY(MonsterBase, ServerObject)
 
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
    State state_;
 
};