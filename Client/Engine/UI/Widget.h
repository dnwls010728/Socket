#pragma once
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

namespace UI
{
    class Widget : public std::enable_shared_from_this<Widget>
    {
        SHADER_CLASS_HELPER(Widget)
        GENERATED_BODY(Widget)
        
    public:
        Widget();
        virtual ~Widget() = default;

        void Tick(float delta_time);
        void Render();
    
    };
}
