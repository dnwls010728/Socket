#pragma once

struct EventData;

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;

    virtual void OnNotify(const EventData&) = 0;

protected:
    ISubscriber() = default;
    
};
