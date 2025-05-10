#pragma once
#include "GameEngine.h"
#include "Singleton.h"
#include "Misc/Function.h"

struct TimerData;
struct TimerHandle;

enum class TimerStatus : uint64_t
{
    kPending,
    kActive,
    kPaused,
    kExecuting,
    kRemoval
};

struct TimerHandle
{
    TimerHandle() :
        handle(0)
    {}

    bool IsValid() const
    {
        return handle != 0;
    }

    void Invalidate()
    {
        handle = 0;
    }

    bool operator==(const TimerHandle& kOther) const
    {
        return handle == kOther.handle;
    }

    bool operator!=(const TimerHandle& kOther) const
    {
        return handle != kOther.handle;
    }

    uint64_t handle;
};

struct TimerData
{
    TimerData() = delete;

    TimerData(Function<void(void)>&& func) :
        callback(std::forward<Function<void(void)>>(func)), loop(false), rate(0.f), expire_time(0.f), status(TimerStatus::kActive)
    {}

    template<typename M>
    TimerData(M* target, Function<void(void)>&& func) :
        callback(std::forward<Function<void(void)>>(target, func)), loop(false), rate(0.f), expire_time(0.f), status(TimerStatus::kActive)
    {}

    bool operator==(const TimerData& kOther) const
    {
        return kOther.handle == handle;
    }

    bool loop;
    float rate;
    double expire_time;
    Function<void(void)> callback;
    TimerHandle handle;
    TimerStatus status;
};

class TimerManager : public Singleton<TimerManager>
{
public:
    TimerManager();
    virtual ~TimerManager() override = default;

    template<typename M>
    void SetTimer(TimerHandle& handle, M* target, void(M::* func)(void), float rate, bool loop = false, float delay = -1.f, typename std::enable_if<std::is_class<M>::value>::type* = nullptr);
    
    template<typename M>
    void SetTimer(TimerHandle& handle, M* target, void(M::* func)(void) const, float rate, bool loop = false, float delay = -1.f, typename std::enable_if<std::is_class<M>::value>::type* = nullptr);
    
    template<typename L>
    void SetTimer(TimerHandle& handle, L&& lambda, float rate, bool loop = false, float delay = -1.f);
    
    void SetTimer(TimerHandle& handle, Function<void(void)>&& func, float rate, bool loop = false, float delay = -1.f);
    void SetTimer(TimerHandle& handle, void(*func)(void), float rate, bool loop = false, float delay = -1.f);

    void ClearTimer(TimerHandle& handle);
    void PauseTimer(TimerHandle handle);
    void UnPauseTimer(TimerHandle handle);
    void ClearAllTimers();

    TimerData& GetTimer(const TimerHandle& handle);
    TimerData* FindTimer(const TimerHandle& kHandle);

    float GetTimerElapsed(TimerHandle handle);
    float GetTimerRemaining(TimerHandle handle);

    bool IsTimerActive(TimerHandle handle);
    bool IsTimerPaused(TimerHandle handle);

    FORCEINLINE bool HasBeenTickedThisFrame() const { return last_ticked_frame_ == g_frame_counter; }
private:
    friend class World;
    
    void SetTimer_Internal(TimerHandle& handle, TimerData& data, float rate, bool loop, float delay);
    void ClearTimer_Internal(TimerHandle handle);
    
    void Tick(float delta_time);
    void RemoveTimer(TimerHandle handle);
    
    uint64_t last_ticked_frame_;
    
    float internal_time_;
    
    std::vector<TimerData> timers_;
    std::vector<TimerHandle> active_timers_;
    std::vector<TimerHandle> paused_timers_;
    std::vector<TimerHandle> pending_timers_;
    
    TimerHandle executing_timer_;

    static uint64_t last_handle_;
    
};

template<typename M>
void TimerManager::SetTimer(TimerHandle& handle, M* target, void(M::* func)(void), float rate, bool loop, float delay, typename std::enable_if<std::is_class<M>::value>::type*)
{
    TimerData data(std::move(Function<void(void)>(target, func)));
    SetTimer_Internal(handle, data, rate, loop, delay);
}

template<typename M>
void TimerManager::SetTimer(TimerHandle& handle, M* target, void(M::* func)(void) const, float rate, bool loop, float delay, typename std::enable_if<std::is_class<M>::value>::type*)
{
    TimerData data(std::move(Function<void(void)>(target, func)));
    SetTimer_Internal(handle, data, rate, loop, delay);
}

template<typename L>
void TimerManager::SetTimer(TimerHandle& handle, L&& lambda, float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(std::move(lambda))));
    SetTimer_Internal(handle, data, rate, loop, delay);
}
