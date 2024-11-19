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
    bool ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Type::uint32 handler_result);

private:
    friend class Core;
    
    void Clear();
    
    std::queue<Event> events_;
    
};
