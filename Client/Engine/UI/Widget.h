#pragma once
#include "Input/Mouse.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

class Renderer;

namespace UI
{
    class Widget : public std::enable_shared_from_this<Widget>
    {
        SHADER_CLASS_HELPER(Widget)
        GENERATED_BODY(Widget)
        
    public:
        Widget(const std::wstring& kName);
        virtual ~Widget() = default;

        Math::Rect GetRect() const;

        Math::Vector2 GetPivotPosition() const;

        bool Contains(const Math::Vector2& kPosition) const;

        FORCEINLINE const std::wstring& GetName() const { return name_; }

        FORCEINLINE const Math::Vector2& GetPosition() const { return position_; }
        FORCEINLINE void SetPosition(const Math::Vector2& kPosition) { position_ = kPosition; }

        FORCEINLINE const Math::Vector2& GetSize() const { return size_; }
        FORCEINLINE void SetSize(const Math::Vector2& kSize) { size_ = kSize; }

        FORCEINLINE const Math::Vector2& GetPivot() const { return pivot_; }
        FORCEINLINE void SetPivot(const Math::Vector2& kPivot) { pivot_ = kPivot; }

        FORCEINLINE bool IsFocused() const { return is_focused_; }

        static std::shared_ptr<Widget> Create(const std::wstring& kName);

    protected:
        friend class Manager;

        virtual void Tick(float delta_time);
        virtual void Render(Renderer* renderer, WindowsWindow* window);

        virtual bool OnMouseEnter();
        virtual bool OnMouseLeave();
        virtual bool OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed);
        virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
        virtual bool OnDragBegin(const Math::Vector2& kPosition);
        virtual bool OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta);
        virtual bool OnDragEnd(const Math::Vector2& kPosition);
        virtual bool OnDrop(const std::shared_ptr<Widget>& kWidget, const Math::Vector2& kPosition);
        virtual bool OnKey(Type::uint16 key_code, bool is_pressed);
        virtual bool OnChar(wchar_t character);
        
        virtual void OnFocus(bool is_focus);

        std::wstring name_;

        Math::Vector2 position_;
        Math::Vector2 size_;
        Math::Vector2 pivot_;

        bool is_focused_;
    
    };
}
