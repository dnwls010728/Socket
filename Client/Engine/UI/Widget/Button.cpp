#include "pch.h"
#include "Button.h"

#include "Level/World.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

Button::Button(const std::wstring& kName) :
    Widget(kName),
    texture_(nullptr),
    draw_mode_(DrawMode::kSimple)
{
    size_ = { 150.f, 50.f };
    is_ray_cast_target_ = true;
}

void Button::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    if (texture_)
    {
        if (draw_mode_ == DrawMode::kSimple)
            renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_);
        else renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_, true, texture_->GetSlice9Rect());
    }
    
    Widget::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Button>("Button")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
