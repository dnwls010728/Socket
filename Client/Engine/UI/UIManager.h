#pragma once
#include "Math/Vector2.h"

namespace UI
{
    class Widget;

    class Manager : public Singleton<Manager>
    {
    public:
        Manager();
        virtual ~Manager() override = default;

        void AddToViewport(const std::shared_ptr<Widget>& widget);
        void RemoveFromViewport(const std::shared_ptr<Widget>& widget);
        void SetFocus(const std::shared_ptr<Widget>& widget);

        bool IsInViewport(const std::shared_ptr<Widget>& widget);

    private:
        friend class Core;
        friend class GameEngine;
        
        void Tick(float delta_time);
        void Render();

        void OnEvent(const Event& kEvent);
        
        std::vector<std::shared_ptr<Widget>> widgets_;
        std::weak_ptr<Widget> focused_widget_;

        Math::Vector2 last_mouse_position_;
    
    };
}

