#pragma once
#include "CharacterBase.h"

class Npc : public CharacterBase
{
    SHADER_CLASS_HELPER(Npc)
    GENERATED_BODY(Npc, CharacterBase)
    
public:
    Npc(const std::wstring& name);
    virtual ~Npc() override = default;

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;
    virtual void OnSpeakEnd() override;

private:
    float hide_duration_;
    float show_duration_;

    bool is_showing_;

    TimerHandle timer_handle_;

    UINameTag* sub_name_tag_;
    
};
