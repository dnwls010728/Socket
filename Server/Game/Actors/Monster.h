#pragma once
#include "Actor.h"

class Monster : public Actor
{
public:
    SHADER_CLASS_HELPER(Monster)
    GENERATED_BODY(Monster)
    
    Monster(std::wstring kName);
protected:
    void Tick(float delta_time) override;
    
    
    
};
