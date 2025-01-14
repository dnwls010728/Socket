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
        void AttachWidget(const std::shared_ptr<Widget>& widget);
        void DetachWidget(const std::shared_ptr<Widget>& widget);
        void AttachWidgets();
        void DetachWidgets();

        std::vector<std::shared_ptr<Widget>> widgets_;
        std::vector<std::shared_ptr<Widget>> pending_widgets_;
        std::vector<std::shared_ptr<Widget>> pending_detach_widgets_;
    
    };
}
