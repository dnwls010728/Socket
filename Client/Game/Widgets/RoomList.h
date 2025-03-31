#pragma once
#include "UI/Widget.h"

namespace UI
{
    class RoomList : public Widget
    {
        SHADER_CLASS_HELPER(RoomList)
        GENERATED_BODY(RoomList, Widget)
        
    public:
        RoomList(const std::wstring& kName);
        virtual ~RoomList() override = default;

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kVelocity) override;

        bool is_over_;

        float scroll_offset_y_;
    
    };
}
