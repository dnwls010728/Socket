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

        void AttachCanvas();
        void DetachCanvas();
        void AttachParent(Widget* parent);
        void DetachParent();

    protected:
        friend class Canvas;
        
        virtual void Render();

        Widget* parent_;
        std::vector<std::shared_ptr<Widget>> children_;
    
    };
}
