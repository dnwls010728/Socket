#include "pch.h"
#include "Image.h"

#include "Level/World.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

Image::Image(const std::wstring& kName) :
    Widget(kName),
    texture_(nullptr),
    draw_mode_(DrawMode::kSimple)
{
}

void Image::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    if (draw_mode_ == DrawMode::kSimple)
        renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_);
    else renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_, true, texture_->GetSlice9Rect());
    
    Widget::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Image>("Image")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
