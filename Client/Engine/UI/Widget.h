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
        Widget(const std::wstring& kName);
        virtual ~Widget() = default;

        void AttachToWidget(Widget* parent);
        void DetachFromWidget();

        FORCEINLINE const std::wstring& GetName() const { return name_; }

        static std::shared_ptr<Widget> Create(const std::wstring& kName);

    protected:
        friend class Manager;
        
        void Tick(float delta_time);
        void Render();

        std::wstring name_;
        
        Widget* parent_;
        std::vector<std::shared_ptr<Widget>> children_;
    
    };
}
