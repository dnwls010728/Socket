#pragma once
#include <queue>
#include <Windows.h>

#include "Singleton.h"
#include "Misc/Type.h"
#include "Math/Vector2.h"

union Event;

enum class MouseButton
{
    kLeft,
    kRight,
    kMiddle
};

struct MouseState
{
    bool is_down;
    bool was_down;

    MouseState() :
        is_down(false),
        was_down(false)
    {
    }
};

class Mouse : public Singleton<Mouse>
{
public:
    Mouse();
    virtual ~Mouse() override = default;

    bool GetMouseButton(MouseButton button) const;
    bool GetMouseButtonDown(MouseButton button) const;
    bool GetMouseButtonUp(MouseButton button) const;

    inline int GetWheelAxis() const { return wheel_axis_; }
    inline int GetWheelHAxis() const { return wheel_h_axis_; }
    
    inline Math::Vector2 GetMousePosition() const { return mouse_position_; }

private:
    friend class Core;

    void OnEvent(const Event& kEvent);
    void UpdateButtonStates();
    void Clear();

    MouseState mouse_states_[3];

    int wheel_axis_;
    int wheel_h_axis_;
    
    Math::Vector2 mouse_position_;

    // 스레드로 부터 안전을 위한 뮤텍스
    std::mutex mutex_;
    
};
