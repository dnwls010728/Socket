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

    FORCEINLINE void SetMessageTick(uint32_t tick) { message_tick_.store(tick); }

    double GetEventTimestamp() const;

private:
    friend class Core;
    
    void Clear();

    std::mutex mutex_;
    
    std::queue<Event> events_;

    std::atomic_uint32_t message_tick_;
    
};
