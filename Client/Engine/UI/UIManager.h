#pragma once
#include "DirectXTK/SimpleMath.h"
#include "Math/Vector2.h"

namespace UI_OLD
{
    class Widget;

    class Manager : public Singleton<Manager>
    {
    public:
        Manager();
        virtual ~Manager() override = default;

        void SetFocus(const std::shared_ptr<Widget>& kWidget);
        
        FORCEINLINE bool HasFocus() const { return focused_widget_.lock() != nullptr; }

        std::shared_ptr<Widget> RayCast(const Math::Vector2& kPosition) const;

    private:
        friend class Core;
        friend class GameEngine;
        friend class Widget;
        
        void Tick(float delta_time);
        void Render();

        void OnEvent(const Event& kEvent);
        
        std::vector<std::shared_ptr<Widget>> widgets_;
        
        std::weak_ptr<Widget> focused_widget_;
        std::weak_ptr<Widget> dragging_widget_;

        Math::Vector2 last_mouse_position_;

        bool is_dragging_;
        bool has_begun_drag_;
    
    };
}

