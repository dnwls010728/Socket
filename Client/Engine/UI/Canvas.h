#pragma once
#include "Singleton.h"
#include "Event/Events.h"

namespace UI
{
    class Canvas : public Singleton<Canvas>
    {
    public:
        Canvas();
        virtual ~Canvas() override = default;

    private:
        friend class Core;
        friend class GameEngine;
        
        void OnEvent(const Event& kEvent);
        void Tick(float delta_time);
        void Render();
    
    };
}
