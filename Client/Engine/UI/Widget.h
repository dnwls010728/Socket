#pragma once
#include "Input/Mouse.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
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

        Math::Rect GetRect() const;

        bool Contains(const Math::Vector2& kPosition) const;

        FORCEINLINE const std::wstring& GetName() const { return name_; }

        FORCEINLINE const Math::Vector2& GetPosition() const { return position_; }
        FORCEINLINE void SetPosition(const Math::Vector2& kPosition) { position_ = kPosition; }

        FORCEINLINE const Math::Vector2& GetSize() const { return size_; }
        FORCEINLINE void SetSize(const Math::Vector2& kSize) { size_ = kSize; }

        FORCEINLINE const std::vector<std::shared_ptr<Widget>>& GetChildren() const { return children_; }

        static std::shared_ptr<Widget> Create(const std::wstring& kName);

    protected:
        friend class Manager;

        virtual void Tick(float delta_time);
        virtual void Render();

        virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed);

        std::wstring name_;
        
        Widget* parent_;
        std::vector<std::shared_ptr<Widget>> children_;

        Math::Vector2 position_;
        Math::Vector2 size_;
    
    };
}
