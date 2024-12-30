#include "pch.h"
#include "TimerManager.h"

#include "Logger.h"
#include "Misc/Type.h"

Type::uint64 TimerManager::last_handle_ = 0;

TimerManager::TimerManager() :
    last_ticked_frame_(-1),
    internal_time_(0.f),
    timers_(),
    active_timers_(),
    pending_timers_()
{
}

void TimerManager::Tick(float delta_time)
{
    internal_time_ += delta_time;

    for (auto it = timers_.begin(); it != timers_.end();)
    {
        TimerData& timer = *it;
    
        if (timer.status == TimerStatus::kRemoval)
        {
            it = timers_.erase(it);
            continue;
        }

        if (timer.status == TimerStatus::kActive && internal_time_ >= timer.expire_time)
        {
            timer.status = TimerStatus::kExecuting;
        
            Type::uint32 cell_count = timer.loop ? static_cast<int>(trunc(internal_time_ - timer.expire_time) / timer.rate) + 1 : 1;
            for (Type::uint32 i = 0; i < cell_count; ++i)
            {
                timer.callback();
            }

            if (timer.loop)
            {
                timer.expire_time += timer.rate * cell_count;
                timer.status = TimerStatus::kActive;
                ++it;
            }
            else
            {
                it = timers_.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }
    
    last_ticked_frame_ = g_frame_counter;
}

const TimerHandle& TimerManager::SetTimer(Function<void(void)>&& func, float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(func)));
    return SetTimer_Internal(data, rate, loop, delay);
}

const TimerHandle& TimerManager::SetTimer(void(*func)(void), float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(func)));
    return SetTimer_Internal(data, rate, loop, delay);
}

void TimerManager::ClearTimer(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);

    switch (timer->status)
    {
    case TimerStatus::kActive:
        {
            timer->status = TimerStatus::kRemoval;
        }
        break;

    case TimerStatus::kExecuting:
    case TimerStatus::kPaused:
        {
            RemoveTimer(*timer);
        }
        break;
    }
}

void TimerManager::PauseTimer(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    if (!timer || timer->status == TimerStatus::kPaused) return;

    if (timer->status == TimerStatus::kExecuting && !timer->loop)
    {
        RemoveTimer(*timer);
    }
    else
    {
        timer->status = TimerStatus::kPaused;
        timer->expire_time -= internal_time_;
    }
}

void TimerManager::UnPauseTimer(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    if (!timer || timer->status != TimerStatus::kPaused) return;

    timer->expire_time += internal_time_;
    timer->status = TimerStatus::kActive;
}

void TimerManager::ClearAllTimers()
{
    // 추후 개발하면서 상황을 보고, 수정해야 할 수 있음
    for (auto& timer : timers_)
    {
        timer.status = TimerStatus::kRemoval;
    }
}

TimerData* TimerManager::FindTimer(const TimerHandle& kHandle)
{
    for (auto& timer : timers_)
    {
        if (timer.handle == kHandle)
        {
            return &timer;
        }
    }
    return nullptr;
}

float TimerManager::GetTimerElapsed(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    if (timer)
    {
        switch (timer->status)
        {
        case TimerStatus::kActive:
        case TimerStatus::kExecuting:
            return timer->rate - (timer->expire_time - internal_time_);

        default:
            return timer->rate - timer->expire_time;
        }
    }
    
    return -1.f;
}

float TimerManager::GetTimerRemaining(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    if (timer)
    {
        switch (timer->status)
        {
        case TimerStatus::kActive:
            return timer->expire_time - internal_time_;
            
        case TimerStatus::kExecuting:
            return 0.f;

        default:
            return timer->expire_time;
        }
    }
    
    return -1.f;
}

bool TimerManager::IsTimerActive(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    return timer && timer->status != TimerStatus::kPaused;
}

bool TimerManager::IsTimerPaused(const TimerHandle& kHandle)
{
    TimerData* timer = FindTimer(kHandle);
    return timer && timer->status == TimerStatus::kPaused;
}

const TimerHandle& TimerManager::SetTimer_Internal(TimerData& data, float rate, bool loop, float delay)
{
    TimerHandle handle;
    handle.handle = ++last_handle_;
    
    const float first_delay = delay >= 0.f ? delay : rate;
    data.loop = loop;
    data.rate = rate;
    data.handle = handle;

    if (HasBeenTickedThisFrame())
    {
        data.expire_time = internal_time_ + first_delay;
        data.status = TimerStatus::kActive;
        active_timers_.push_back(data.handle);
    }
    else
    {
        data.expire_time = first_delay;
        data.status = TimerStatus::kPending;
        pending_timers_.push_back(data.handle);
    }
    
    timers_.push_back(data);
    return data.handle;
}

void TimerManager::RemoveTimer(const TimerData& kTimer)
{
    timers_.erase(std::ranges::find(timers_, kTimer));
}
