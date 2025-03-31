#pragma once
#include "UI/Widget.h"

namespace UI
{
    class ServerList : public Widget
    {
        SHADER_CLASS_HELPER(ServerList)
        GENERATED_BODY(ServerList, Widget)
        
    public:
        ServerList(const std::wstring& kName);
        virtual ~ServerList() override = default;

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;
        
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kVelocity) override;

        float scroll_offset_y_;
    
    };
}
