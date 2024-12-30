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
    paused_timers_(),
    pending_timers_()
{
}

void TimerManager::Tick(float delta_time)
{
    internal_time_ += delta_time;

    for (auto it = active_timers_.begin(); it != active_timers_.end();)
    {
        TimerHandle handle = *it;
        TimerData* timer_data = nullptr;
        for (auto& timer : timers_)
        {
            if (timer.handle == handle)
            {
                timer_data = &timer;
                break;
            }
        }
    
        if (timer_data->status == TimerStatus::kRemoval)
        {
            RemoveTimer(handle);
            it = active_timers_.erase(it);
            continue;
        }

        if (internal_time_ >= timer_data->expire_time)
        {
            timer_data->status = TimerStatus::kExecuting;
        
            Type::uint32 cell_count = timer_data->loop ? static_cast<int>(trunc(internal_time_ - timer_data->expire_time) / timer_data->rate) + 1 : 1;
            for (Type::uint32 i = 0; i < cell_count; ++i)
            {
                timer_data->callback();
            }

            if (timer_data->loop)
            {
                timer_data->expire_time += timer_data->rate * cell_count;
                timer_data->status = TimerStatus::kActive;
                ++it;
            }
            else
            {
                RemoveTimer(handle);
                it = active_timers_.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }
    
    last_ticked_frame_ = g_frame_counter;

    for (auto& handle : pending_timers_)
    {
        TimerData& timer_data = GetTimer(handle);

        timer_data.expire_time += internal_time_;
        timer_data.status = TimerStatus::kActive;
        active_timers_.push_back(handle);
    }

    pending_timers_.clear();
}

void TimerManager::SetTimer(TimerHandle& handle, Function<void(void)>&& func, float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(func)));
    return SetTimer_Internal(handle, data, rate, loop, delay);
}

void TimerManager::SetTimer(TimerHandle& handle, void(*func)(void), float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(func)));
    return SetTimer_Internal(handle, data, rate, loop, delay);
}

void TimerManager::ClearTimer(TimerHandle& handle)
{
    if (const TimerData* kTimer = FindTimer(handle))
    {
        ClearTimer_Internal(handle);
    }

    handle.Invalidate();
}

void TimerManager::PauseTimer(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
    if (!timer || timer->status == TimerStatus::kPaused) return;

    TimerStatus status = timer->status;

    switch (status)
    {
    case TimerStatus::kActive:
        {
            std::erase(active_timers_, handle);
        }
        break;

    case TimerStatus::kPending:
        {
            std::erase(pending_timers_, handle);
        }
        break;
    }

    if (status == TimerStatus::kExecuting && !timer->loop)
    {
        RemoveTimer(handle);
    }
    else
    {
        paused_timers_.push_back(handle);
        
        timer->status = TimerStatus::kPaused;

        if (status != TimerStatus::kPending)
        {
            timer->expire_time -= internal_time_;
        }
    }
}

void TimerManager::UnPauseTimer(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
    if (!timer || timer->status != TimerStatus::kPaused) return;

    if (HasBeenTickedThisFrame())
    {
        timer->expire_time += internal_time_;
        timer->status = TimerStatus::kActive;
        active_timers_.push_back(handle);
    }
    else
    {
        timer->status = TimerStatus::kPending;
        pending_timers_.push_back(handle);
    }

    std::erase(paused_timers_, handle);
}

void TimerManager::ClearAllTimers()
{
    for (auto& timer : timers_)
    {
        ClearTimer(timer.handle);
    }
}

TimerData& TimerManager::GetTimer(const TimerHandle& handle)
{
    for (auto& timer : timers_)
    {
        if (timer.handle == handle)
        {
            return timer;
        }
    }

    CHECK(false);
}

TimerData* TimerManager::FindTimer(const TimerHandle& kHandle)
{
    if (!kHandle.IsValid()) return nullptr;
    
    for (auto& timer : timers_)
    {
        if (timer.handle == kHandle)
        {
            if (timer.status == TimerStatus::kRemoval) return nullptr;
            return &timer;
        }
    }
    
    return nullptr;
}

float TimerManager::GetTimerElapsed(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
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

float TimerManager::GetTimerRemaining(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
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

bool TimerManager::IsTimerActive(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
    return timer && timer->status != TimerStatus::kPaused;
}

bool TimerManager::IsTimerPaused(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);
    return timer && timer->status == TimerStatus::kPaused;
}

void TimerManager::SetTimer_Internal(TimerHandle& handle, TimerData& data, float rate, bool loop, float delay)
{
    if (FindTimer(handle))
    {
        ClearTimer_Internal(handle);
    }

    if (rate > 0.f)
    {
        data.loop = loop;
        data.rate = rate;
        
        const float first_delay = delay >= 0.f ? delay : rate;
        
        TimerHandle new_handle;
        new_handle.handle = ++last_handle_;
        data.handle = new_handle;

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
    
        handle = new_handle;
        timers_.push_back(data);
    }
    else
    {
        handle.Invalidate();
    }
}

void TimerManager::ClearTimer_Internal(TimerHandle handle)
{
    TimerData* timer = FindTimer(handle);

    switch (timer->status)
    {
    case TimerStatus::kPending:
        {
            std::erase(pending_timers_, handle);
            RemoveTimer(handle);
        }
        break;
        
    case TimerStatus::kActive:
        {
            timer->status = TimerStatus::kRemoval;
        }
        break;

    case TimerStatus::kExecuting:
    case TimerStatus::kPaused:
        {
            RemoveTimer(handle);
        }
        break;
    }
}

void TimerManager::RemoveTimer(TimerHandle handle)
{
    const TimerData* kTimer = FindTimer(handle);
    if (!kTimer) return;

    std::erase(timers_, *kTimer);
}
