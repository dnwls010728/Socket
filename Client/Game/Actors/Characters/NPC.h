#pragma once
#include "CharacterBase.h"
#include "Utils/TimedBool.h"

class NPC : public CharacterBase
{
    SHADER_CLASS_HELPER(NPC)
    GENERATED_BODY(NPC, CharacterBase)
    
public:
    NPC(const std::wstring& name);
    virtual ~NPC() override = default;

    void Init(uint32_t npc_id, const Math::Vector2& position);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;
    virtual void OnSpeakEnd() override;
    
    uint32_t npc_id_;

    std::vector<std::wstring> speeches_;
    
    float hide_duration_;
    float show_duration_;
    float last_pressed_time_;

    bool is_showing_;

    TimerHandle timer_handle_;
    
};
