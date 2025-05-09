#pragma once
#include <queue>
#include <Windows.h>

#include "Events.h"
#include "Singleton.h"

class EventManager : public Singleton<EventManager>
{
public:
    EventManager();
    virtual ~EventManager() override = default;

    bool PollEvent(Event& event);
    bool ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, uint32_t handler_result);

    FORCEINLINE void SetMessageTick(DWORD tick) { message_tick_ = tick; }

    double GetEventTimestamp();

private:
    friend class Core;
    
    void Clear();
    
    std::queue<Event> events_;

    DWORD message_tick_;
    
};
