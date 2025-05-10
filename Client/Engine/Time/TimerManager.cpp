#include "pch.h"
#include "TimerManager.h"

uint64_t TimerManager::last_handle_ = 0;

TimerManager::TimerManager() :
    last_ticked_frame_(-1),
    internal_time_(0.f),
    timers_(),
    active_timers_(),
    paused_timers_(),
    pending_timers_(),
    executing_timer_()
{
}

void TimerManager::Tick(float delta_time)
{
    internal_time_ += delta_time;

    for (auto it = active_timers_.begin(); it != active_timers_.end();)
    {
        TimerHandle handle = *it;
        TimerData* data = nullptr;
        for (auto& timer : timers_)
        {
            if (timer.handle == handle)
            {
                data = &timer;
                break;
            }
        }
    
        if (data->status == TimerStatus::kRemoval)
        {
            RemoveTimer(handle);
            it = active_timers_.erase(it);
            continue;
        }

        if (internal_time_ >= data->expire_time)
        {
            executing_timer_ = handle;
            it = active_timers_.erase(it);
            
            data->status = TimerStatus::kExecuting;
        
            uint32_t cell_count = data->loop ? static_cast<int32_t>(std::trunc(internal_time_ - data->expire_time) / data->rate) + 1 : 1;
            for (uint32_t i = 0; i < cell_count; ++i)
            {
                data->callback();

                data = FindTimer(executing_timer_);
                if (!data || data->status != TimerStatus::kExecuting) break;
            }

            if (data)
            {
                if (data->loop)
                {
                    data->expire_time += data->rate * cell_count;
                    data->status = TimerStatus::kActive;
                    it = active_timers_.insert(it, handle);
                }
                else
                {
                    RemoveTimer(handle);
                }

                executing_timer_.Invalidate();
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
    TimerData* data = FindTimer(handle);
    if (!data || data->status == TimerStatus::kPaused) return;

    TimerStatus status = data->status;

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

        case TimerStatus::kExecuting:
        {
            CHECK(executing_timer_ == handle);
                
            executing_timer_.Invalidate();
        }
    }

    if (status == TimerStatus::kExecuting && !data->loop)
    {
        RemoveTimer(handle);
    }
    else
    {
        paused_timers_.push_back(handle);
        
        data->status = TimerStatus::kPaused;

        if (status != TimerStatus::kPending)
        {
            data->expire_time -= internal_time_;
        }
    }
}

void TimerManager::UnPauseTimer(TimerHandle handle)
{
    TimerData* data = FindTimer(handle);
    if (!data || data->status != TimerStatus::kPaused) return;

    if (HasBeenTickedThisFrame())
    {
        data->expire_time += internal_time_;
        data->status = TimerStatus::kActive;
        active_timers_.push_back(handle);
    }
    else
    {
        data->status = TimerStatus::kPending;
        pending_timers_.push_back(handle);
    }

    std::erase(paused_timers_, handle);
}

void TimerManager::ClearAllTimers()
{
    // 추후 재확인이 필요할 수 있음
    timers_.clear();
    active_timers_.clear();
    paused_timers_.clear();
    pending_timers_.clear();
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
    TimerData* data = FindTimer(handle);
    if (data)
    {
        switch (data->status)
        {
        case TimerStatus::kActive:
        case TimerStatus::kExecuting:
            return data->rate - (data->expire_time - internal_time_);

        default:
            return data->rate - data->expire_time;
        }
    }
    
    return -1.f;
}

float TimerManager::GetTimerRemaining(TimerHandle handle)
{
    TimerData* data = FindTimer(handle);
    if (data)
    {
        switch (data->status)
        {
        case TimerStatus::kActive:
            return data->expire_time - internal_time_;
            
        case TimerStatus::kExecuting:
            return 0.f;

        default:
            return data->expire_time;
        }
    }
    
    return -1.f;
}

bool TimerManager::IsTimerActive(TimerHandle handle)
{
    TimerData* data = FindTimer(handle);
    return data && data->status != TimerStatus::kPaused;
}

bool TimerManager::IsTimerPaused(TimerHandle handle)
{
    TimerData* data = FindTimer(handle);
    return data && data->status == TimerStatus::kPaused;
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
    TimerData& data = GetTimer(handle);

    switch (data.status)
    {
    case TimerStatus::kPending:
        {
            std::erase(pending_timers_, handle);
            RemoveTimer(handle);
        }
        break;
        
    case TimerStatus::kActive:
        {
            data.status = TimerStatus::kRemoval;
        }
        break;

    case TimerStatus::kPaused:
        {
            std::erase(paused_timers_, handle);
            RemoveTimer(handle);
        }
        break;

    case TimerStatus::kExecuting:
        {
            CHECK(executing_timer_ == handle);
            
            executing_timer_.Invalidate();
            RemoveTimer(handle);
        }
        break;
    }
}

void TimerManager::RemoveTimer(TimerHandle handle)
{
    const TimerData& kData = GetTimer(handle);
    std::erase(timers_, kData);
}
