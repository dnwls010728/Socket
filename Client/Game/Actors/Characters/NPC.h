#pragma once
#include "CharacterBase.h"

class NPC : public CharacterBase
{
    SHADER_CLASS_HELPER(NPC)
    GENERATED_BODY(NPC, CharacterBase)
    
public:
    NPC(const std::wstring& name);
    virtual ~NPC() override = default;

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
