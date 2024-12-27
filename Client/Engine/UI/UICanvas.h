#pragma once
#include "Singleton.h"

union Event;

class UICanvas : public Singleton<UICanvas>
{
public:
    UICanvas();
    virtual ~UICanvas() override = default;

private:
    friend class Core;
    friend class GameEngine;

    void OnEvent(const Event& event);
    
    void Render();
    
};
