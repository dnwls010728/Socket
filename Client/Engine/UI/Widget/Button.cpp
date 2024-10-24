#include "pch.h"
#include "Button.h"

#include "Level/World.h"
#include "Math/Color.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

UI::Button::Button(const std::wstring& kName) :
    Widget(kName),
    color_(Math::Color::White)
{
    size_ = {160.f, 30.f};
    can_interact_ = true;
}

void UI::Button::OnMousePressed()
{
    Widget::OnMousePressed();

    color_ = Math::Color::Green;
    
}

void UI::Button::OnMouseReleased()
{
    Widget::OnMouseReleased();

    on_click.Execute();
    
}

void UI::Button::OnMouseHover()
{
    Widget::OnMouseHover();

    color_ = Math::Color::Red;
    
}

void UI::Button::OnMouseLeave()
{
    Widget::OnMouseLeave();

    color_ = Math::Color::White;
    
}

void UI::Button::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;
    
    Renderer* renderer = Renderer::Get();
    if (!renderer) return;

    Math::Vector2 pivot_position = GetPivotPosition();
    if (GetParent()) pivot_position = GetParent()->GetPivotPosition();

    renderer->DrawBox(window, rect_, pivot_position, color_, angle_);
    
}
