#include "pch.h"
#include "Mouse.h"

#include "Event/Events.h"

Mouse::Mouse() :
    mouse_states_{},
    wheel_axis_(0),
    wheel_h_axis_(0),
    mouse_position_(Math::Vector2::Zero())
{
}

bool Mouse::GetMouseButton(MouseButton button) const
{
    MouseState state = mouse_states_[static_cast<int>(button)];
    return state.is_down;
}

bool Mouse::GetMouseButtonDown(MouseButton button) const
{
    MouseState state = mouse_states_[static_cast<int>(button)];
    return state.is_down && !state.was_down;
}

bool Mouse::GetMouseButtonUp(MouseButton button) const
{
    MouseState state = mouse_states_[static_cast<int>(button)];
    return !state.is_down && state.was_down;
}

void Mouse::OnEvent(const Event& kEvent)
{
    const Type::uint32& kType = kEvent.type;
    if (kType == static_cast<Type::uint32>(EventType::kMouseMotion))
    {
        const MouseMotionEvent& kMotion = kEvent.motion;
        mouse_position_ = Math::Vector2(kMotion.x, kMotion.y);
    }
    else if (kType & static_cast<Type::uint32>(EventType::kMousePressed) || kType & static_cast<Type::uint32>(EventType::kMouseReleased))
    {
        const MouseButtonEvent& kButton = kEvent.button;
        mouse_states_[static_cast<int>(kButton.button)].is_down = kButton.is_pressed;
    }
    else if (kType == static_cast<Type::uint32>(EventType::kMouseWheel))
    {
        const MouseWheelEvent& kWheel = kEvent.wheel;
        wheel_axis_ = static_cast<int>(kWheel.y);
        wheel_h_axis_ = static_cast<int>(kWheel.x);
    }
}

void Mouse::UpdateButtonStates()
{
    for (auto& mouse_state : mouse_states_)
    {
        mouse_state.was_down = mouse_state.is_down;
    }

    wheel_axis_ = 0;
    wheel_h_axis_ = 0;
}

void Mouse::Clear()
{
    std::lock_guard<std::mutex> lock(mutex_);

    for (auto& mouse_state : mouse_states_)
    {
        mouse_state.is_down = false;
        mouse_state.was_down = false;
    }

    wheel_axis_ = 0;
    wheel_h_axis_ = 0;
}
