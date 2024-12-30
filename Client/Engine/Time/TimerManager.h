#pragma once
#include "GameEngine.h"
#include "Singleton.h"
#include "Misc/Type.h"
#include "Misc/DelegateMacros.h"
#include "Misc/Function.h"

struct TimerData;
struct TimerHandle;

enum class TimerStatus : Type::uint8
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

    Type::uint64 handle;
};

struct TimerData
{
    TimerData() = delete;

    TimerData(Function<void(void)>&& func) 
        : callback(std::forward<Function<void(void)>>(func)), loop(false), rate(0.f), expire_time(0.f), status(TimerStatus::kActive)
    {}

    template<typename M>
    TimerData(M* target, Function<void(void)>&& func)
        : callback(std::forward<Function<void(void)>>(target, func)), loop(false), rate(0.f), expire_time(0.f), status(TimerStatus::kActive)
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
    const TimerHandle& SetTimer(M* target, void(M::* func)(void), float rate, bool loop = false, float delay = -1.f, typename std::enable_if<std::is_class<M>::value>::type* = nullptr);
    
    template<typename M>
    const TimerHandle& SetTimer(M* target, void(M::* func)(void) const, float rate, bool loop = false, float delay = -1.f, typename std::enable_if<std::is_class<M>::value>::type* = nullptr);
    
    template<typename L>
    const TimerHandle& SetTimer(L&& lambda, float rate, bool loop = false, float delay = -1.f);
    
    const TimerHandle& SetTimer(Function<void(void)>&& func, float rate, bool loop = false, float delay = -1.f);
    const TimerHandle& SetTimer(void(*func)(void), float rate, bool loop = false, float delay = -1.f);

    void ClearTimer(const TimerHandle& kHandle);
    void PauseTimer(const TimerHandle& kHandle);
    void UnPauseTimer(const TimerHandle& kHandle);
    void ClearAllTimers();
    
    TimerData* FindTimer(const TimerHandle& kHandle);

    float GetTimerElapsed(const TimerHandle& kHandle);
    float GetTimerRemaining(const TimerHandle& kHandle);

    bool IsTimerActive(const TimerHandle& kHandle);
    bool IsTimerPaused(const TimerHandle& kHandle);

    FORCEINLINE bool HasBeenTickedThisFrame() const { return last_ticked_frame_ == g_frame_counter; }
private:
    friend class World;
    
    const TimerHandle& SetTimer_Internal(TimerData& data, float rate, bool loop, float delay);
    
    void Tick(float delta_time);
    void RemoveTimer(const TimerData& kTimer);
    
    Type::uint64 last_ticked_frame_;
    float internal_time_;
    std::vector<TimerData> timers_;
    std::vector<TimerHandle> active_timers_;
    std::vector<TimerHandle> pending_timers_;

    static Type::uint64 last_handle_;
    
};

template<typename M>
const TimerHandle& TimerManager::SetTimer(M* target, void(M::* func)(void), float rate, bool loop, float delay, typename std::enable_if<std::is_class<M>::value>::type*)
{
    TimerData data(std::move(Function<void(void)>(target, func)));
    return SetTimer_Internal(data, rate, loop, delay);
}

template<typename M>
const TimerHandle& TimerManager::SetTimer(M* target, void(M::* func)(void) const, float rate, bool loop, float delay, typename std::enable_if<std::is_class<M>::value>::type*)
{
    TimerData data(std::move(Function<void(void)>(target, func)));
    return SetTimer_Internal(data, rate, loop, delay);
}

template<typename L>
const TimerHandle& TimerManager::SetTimer(L&& lambda, float rate, bool loop, float delay)
{
    TimerData data(std::move(Function<void(void)>(std::move(lambda))));
    return SetTimer_Internal(data, rate, loop, delay);
}
