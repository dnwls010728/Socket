#pragma once
#include <Windows.h>

#include "Input/Mouse.h"
#include "Misc/EnumClassFlags.h"

enum class EventType : uint32_t
{
    kNone = 0,
    kWindowSize = (0x01<<0),
    kKeyPressed = (0x01<<1),
    kKeyReleased = (0x01<<2),
    kText = (0x01<<3),
    kMousePressed = (0x01<<4),
    kMouseReleased = (0x01<<5),
    kMouseMotion = (0x01<<6),
    kMouseWheel = (0x01<<7),

    kKeyChanged = kKeyPressed | kKeyReleased,
    kMouseChanged = kMousePressed | kMouseReleased,
};

ENUM_CLASS_FLAGS(EventType)

struct WindowEvent
{
    EventType type;
    int32_t data1;
    int32_t data2;
    double timestamp;
};

struct KeyboardEvent
{
    EventType type;
    WORD key_code;
    bool is_repeat;
    double timestamp;
};

struct TextEvent
{
    EventType type;
    wchar_t character;
    double timestamp;
};

struct MouseButtonEvent
{
    EventType type;
    bool is_pressed;
    MouseButton button;
    float x;
    float y;
    double timestamp;
};

struct MouseMotionEvent
{
    EventType type;
    float x;
    float y;
    double timestamp;
};

struct MouseWheelEvent
{
    EventType type;
    float mouse_x;
    float mouse_y;
    float x;
    float y;
    double timestamp;
};

union Event
{
    uint32_t type;
    WindowEvent window;
    KeyboardEvent key;
    TextEvent text;
    MouseButtonEvent button;
    MouseMotionEvent motion;
    MouseWheelEvent wheel;
};
