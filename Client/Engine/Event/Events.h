#pragma once
#include <Windows.h>

#include "Input/Mouse.h"
#include "Misc/EnumClassFlags.h"

enum class EventType : Type::uint32
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
    int data1;
    int data2;
};

struct KeyboardEvent
{
    EventType type;
    WORD key_code;
    bool is_repeat;
};

struct TextEvent
{
    EventType type;
    wchar_t character;
};

struct MouseButtonEvent
{
    EventType type;
    bool is_pressed;
    MouseButton button;
    float x;
    float y;
};

struct MouseMotionEvent
{
    EventType type;
    float x;
    float y;
};

struct MouseWheelEvent
{
    EventType type;
    float mouse_x;
    float mouse_y;
    float x;
    float y;
};

union Event
{
    Type::uint32 type;
    WindowEvent window;
    KeyboardEvent key;
    TextEvent text;
    MouseButtonEvent button;
    MouseMotionEvent motion;
    MouseWheelEvent wheel;
};
