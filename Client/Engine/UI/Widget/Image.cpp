#include "pch.h"
#include "Image.h"

#include "Level/World.h"
#include "Math/Color.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UI::Image::Image(const std::wstring& kName) :
    Widget(kName),
    texture_(nullptr),
    draw_mode_(DrawMode::Simple)
{
}

void UI::Image::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    Renderer* renderer = Renderer::Get();
    if (!renderer) return;

    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    if (!texture_)
    {
        renderer->DrawBox(window, rect_, pivot_position, Math::Color::White, angle_);
        return;
    }

    if (draw_mode_ == DrawMode::Simple)
        renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_);
    else if (draw_mode_ == DrawMode::Sliced)
        renderer->DrawBitmap(window, texture_->GetTexture(), rect_, pivot_position, angle_, true, {50.f, 50.f, 475.f, 475.f});
}
