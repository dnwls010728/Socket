#pragma once
#include "Character/Actor/Mob/MobBase.h"

class Mob1Idle;

class Mob1 : public MobBase
{
    SHADER_CLASS_HELPER(Mob1)
    GENERATED_BODY(Mob1, MobBase)
    
public:
    Mob1(const std::wstring& kName);
    virtual ~Mob1() override = default;

protected:
    virtual void BeginPlay() override;

private:
    void Turn();
    
    std::shared_ptr<Mob1Idle> idle_state_;
    
    class Sprite* sprite_;
    
};
