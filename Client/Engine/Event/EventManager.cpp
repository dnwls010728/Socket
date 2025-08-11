#include "pch.h"
#include "EventManager.h"

#include <windowsx.h>

#include "Input/Mouse.h"
#include "Time/Time.h"

EventManager::EventManager() :
    events_(),
    message_tick_(0)
{
}

bool EventManager::PollEvent(Event& event)
{
    if (events_.empty()) return false;

    event = events_.front();
    events_.pop();

    return true;
}

bool EventManager::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, uint32_t handler_result)
{
    if (message == WM_SIZE)
    {
        if (wParam == SIZE_MINIMIZED) return false;

        int32_t width = LOWORD(lParam);
        int32_t height = HIWORD(lParam);
        
        Event event;
        event.type = static_cast<uint32_t>(EventType::kWindowSize);
        event.window.data1 = width;
        event.window.data2 = height;
        event.window.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }
    
    if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN ||
        message == WM_KEYUP || message == WM_SYSKEYUP)
    {
        WORD key_code = LOWORD(wParam);
        WORD key_flags = HIWORD(lParam);
        
        uint32_t scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xFF));
        if (!scancode) scancode = MapVirtualKey(key_code, MAPVK_VK_TO_VSC_EX);
        Logger::Print(L"Scancode: %d, KeyCode: %d, Flags: %d", scancode, key_code, key_flags);

        bool is_released = (key_flags & KF_UP) == KF_UP;
        bool is_repeat = (key_flags & KF_REPEAT) == KF_REPEAT;

        uint32_t type = 0;
        if (!is_released) type = static_cast<uint32_t>(EventType::kKeyPressed);
        else type = static_cast<uint32_t>(EventType::kKeyReleased);

        Event event;
        event.type = type;
        event.key.scancode = scancode;
        event.key.key_code = key_code;
        event.key.is_repeat = is_repeat;
        event.key.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }
    
    if (message == WM_CHAR)
    {
        const WCHAR kCharacter = static_cast<WCHAR>(wParam);
        if (kCharacter < 32 || (kCharacter > 126 && kCharacter < 160)) return false;

        Event event;
        event.type = static_cast<uint32_t>(EventType::kText);
        event.text.character = kCharacter;
        event.text.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }
    
    if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP ||
        message == WM_RBUTTONDOWN || message == WM_RBUTTONUP ||
        message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
    {
        const int32_t x = GET_X_LPARAM(lParam);
        const int32_t y = GET_Y_LPARAM(lParam);
        
        bool is_pressed = false;
        MouseButton mouse_button = MouseButton::kLeft;
        uint32_t type = 0;

        switch (message)
        {
        case WM_LBUTTONDOWN:
            {
                is_pressed = true;
                mouse_button = MouseButton::kLeft;
            }
            break;

        case WM_LBUTTONUP:
            {
                is_pressed = false;
                mouse_button = MouseButton::kLeft;
            }
            break;

        case WM_RBUTTONDOWN:
            {
                is_pressed = true;
                mouse_button = MouseButton::kRight;
            }
            break;

        case WM_RBUTTONUP:
            {
                is_pressed = false;
                mouse_button = MouseButton::kRight;
            }
            break;

        case WM_MBUTTONDOWN:
            {
                is_pressed = true;
                mouse_button = MouseButton::kMiddle;
            }
            break;

        case WM_MBUTTONUP:
            {
                is_pressed = false;
                mouse_button = MouseButton::kMiddle;
            }
            break;
        }

        if (is_pressed) type = static_cast<uint32_t>(EventType::kMousePressed);
        else type = static_cast<uint32_t>(EventType::kMouseReleased);

        Event event;
        event.type = type;
        event.button.is_pressed = is_pressed;
        event.button.button = mouse_button;
        event.button.x = static_cast<float>(x);
        event.button.y = static_cast<float>(y);
        event.button.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }

    if (message == WM_MOUSEMOVE)
    {
        const int32_t x = GET_X_LPARAM(lParam);
        const int32_t y = GET_Y_LPARAM(lParam);

        Event event;
        event.type = static_cast<uint32_t>(EventType::kMouseMotion);
        event.motion.x = static_cast<float>(x);
        event.motion.y = static_cast<float>(y);
        event.motion.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }

    if (message == WM_MOUSEWHEEL || message == WM_MOUSEHWHEEL)
    {
        POINT mouse_position;
        mouse_position.x = GET_X_LPARAM(lParam);
        mouse_position.y = GET_Y_LPARAM(lParam);

        ScreenToClient(hWnd, &mouse_position);
        
        const short delta = GET_WHEEL_DELTA_WPARAM(wParam);
        float delta_f = static_cast<float>(delta) / static_cast<float>(WHEEL_DELTA);

        Event event;
        event.type = static_cast<uint32_t>(EventType::kMouseWheel);
        event.wheel.mouse_x = static_cast<float>(mouse_position.x);
        event.wheel.mouse_y = static_cast<float>(mouse_position.y);

        if (message == WM_MOUSEWHEEL)
        {
            event.wheel.x = 0.f;
            event.wheel.y = delta_f;
        }
        else
        {
            event.wheel.x = delta_f;
            event.wheel.y = 0.f;
        }

        event.wheel.timestamp = GetEventTimestamp();

        events_.push(event);
        return true;
    }
    
    return false;
}

double EventManager::GetEventTimestamp()
{
    static double timestamp_offset = 0;

    double now = Time::Seconds();
    double timestamp = static_cast<double>(message_tick_) / 1000;
    timestamp += timestamp_offset;

    if (timestamp_offset == 0)
    {
        timestamp_offset = now - timestamp;
        timestamp = now;
    }

    return timestamp;
}

void EventManager::Clear()
{
    events_ = std::queue<Event>();
}
