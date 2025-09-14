#pragma once
#include <map>

#include "Scancode.h"
#include "Singleton.h"

union Event;

// Scancode
DECLARE_DELEGATE(OnKeyDelegate, uint32_t, bool)

struct KeyState
{
    bool is_down;
    bool was_down;

    KeyState() :
        is_down(false),
        was_down(false)
    {
    }
};

class Keyboard : public Singleton<Keyboard>
{
public:
    Keyboard();
    virtual ~Keyboard() override = default;

    bool GetKey(Scancode scancode);
    bool GetKeyDown(Scancode scancode);
    bool GetKeyUp(Scancode scancode);

    OnKeyDelegate key_event;

private:
    friend class Core;
    
    void OnEvent(const Event& kEvent);
    void UpdateKeyStates();
    void Clear();

    std::map<uint32_t, KeyState> key_states_;
    
    // 스레드로 부터 안전을 위한 뮤텍스
    std::mutex mutex_;
    
};
