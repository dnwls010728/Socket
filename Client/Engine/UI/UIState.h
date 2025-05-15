#pragma once
#include "Misc/EngineMacros.h"

class UIState
{
    GENERATED_BODY(UIState)
    
public:
    UIState();
    virtual ~UIState() = default;

protected:
    friend class UI;
    
    FORCEINLINE virtual void Tick(float delta_time) {}
    FORCEINLINE virtual void Render() {}
    FORCEINLINE virtual void OnEvent(const Event& event) {}
    
};
