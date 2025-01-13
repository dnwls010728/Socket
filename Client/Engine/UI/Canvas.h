#pragma once
#include "Singleton.h"
#include "Event/Events.h"

namespace UI
{
    class Widget;
}

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
        friend class Widget;
        
        void OnEvent(const Event& kEvent);
        void Render();

        std::vector<std::shared_ptr<Widget>> widgets_;
    
    };
}
