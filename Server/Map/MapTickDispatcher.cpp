#include "MapTickDispatcher.h"
#include <chrono>
#include "MapBase.h"

using namespace std::chrono;

MapTickDispatcher::MapTickDispatcher() : running_(false), tick_interval_ms_(50), max_maps_per_thread_(10) {}

MapTickDispatcher::~MapTickDispatcher() { Stop(); }

void MapTickDispatcher::Start(uint32_t tick_interval_ms, size_t max_maps_per_thread)
{
    std::lock_guard<std::mutex> lock(worker_mutex_);
    if (running_) return;

    tick_interval_ms_ = tick_interval_ms;
    max_maps_per_thread_ = max_maps_per_thread;
    running_.store(true);
}

void MapTickDispatcher::Stop()
{
    std::lock_guard<std::mutex> lock(worker_mutex_);
    if (!running_) return;

    running_.store(false);
    for (auto& context : worker_contexts_)
        if (context->thread.joinable())
            context->thread.join();

    worker_contexts_.clear();
}

void MapTickDispatcher::AddMap(MapBase* map)
{
    WorkerContext* best = FindBestWorker();
    if (!best)
    {
        auto context = std::make_unique<WorkerContext>();
        WorkerContext* ptr = context.get();
        {
            std::lock_guard<std::mutex> lock(ptr->mutex);
            ptr->assigned_maps.push_back(map);
        }
        worker_contexts_.push_back(std::move(context));
        ptr->thread = std::thread(&MapTickDispatcher::WorkerThread, this, ptr, tick_interval_ms_);
    }
    else
    {
        std::lock_guard<std::mutex> lock(best->mutex);
        best->assigned_maps.push_back(map);
    }
}

void MapTickDispatcher::RemoveMap(MapBase* map)
{
    std::lock_guard<std::mutex> lock(worker_mutex_);

    for (auto it = worker_contexts_.begin(); it != worker_contexts_.end(); ++it)
    {
        auto& context = *it;
        {
            std::lock_guard<std::mutex> map_lock(context->mutex);
            auto& maps = context->assigned_maps;
            maps.erase(std::remove(maps.begin(), maps.end(), map), maps.end());

            if (maps.empty())
            {
                if (context->thread.joinable())
                    context->thread.join();

                std::unique_ptr<WorkerContext> context_temp = std::move(*it);
                worker_contexts_.erase(it);
                break;
            }
        }
    }
    
}

MapTickDispatcher::WorkerContext* MapTickDispatcher::FindBestWorker()
{
    std::lock_guard<std::mutex> lock(worker_mutex_);
    WorkerContext* best = nullptr;
    uint32_t min_duration = UINT32_MAX;

    for (auto& context : worker_contexts_)
    {
        uint32_t duration = context->last_tick_duration_ms.load(std::memory_order_relaxed);
        if (context->assigned_maps.size() < max_maps_per_thread_ && duration < min_duration)
        {
            best = context.get();
            min_duration = duration;
        }
    }

    return best;
}

void MapTickDispatcher::SplitWorker(WorkerContext* context)
{
    std::lock_guard<std::mutex> map_lock(context->mutex);

    if (context->assigned_maps.size() <= 1)
        return;

    // 새로운 WorkerContext 생성
    auto new_context = std::make_unique<WorkerContext>();
    WorkerContext* new_ptr = new_context.get();

    // 절반 이동
    size_t move_count = context->assigned_maps.size() / 2;
    for (size_t i = 0; i < move_count; ++i)
    {
        MapBase* map = context->assigned_maps.back();
        context->assigned_maps.pop_back();
        new_ptr->assigned_maps.push_back(map);
    }

    new_ptr->thread = std::thread(&MapTickDispatcher::WorkerThread, this, new_ptr, tick_interval_ms_);

    {
        std::lock_guard<std::mutex> lock(worker_mutex_);
        worker_contexts_.push_back(std::move(new_context));
    }
}

void MapTickDispatcher::WorkerThread(WorkerContext* context, uint32_t tick_interval_ms)
{
    const milliseconds interval(tick_interval_ms);
    auto last_time = steady_clock::now();
    
    while (running_.load())
    {
        auto start = steady_clock::now();
        duration<float> delta = start - last_time;
        last_time = start;

        {
            std::lock_guard<std::mutex> lock(context->mutex);

            if (context->assigned_maps.empty())
                break;
            
            for (auto* map : context->assigned_maps)
            {
                if (map)
                {
                    map->PhysicsTick(delta.count());
                    map->Tick(delta.count());
                    map->PostTick(delta.count());
                }
            }
        }

        auto elapsed = duration_cast<milliseconds>(steady_clock::now() - start);
        context->last_tick_duration_ms.store(static_cast<uint32_t>(elapsed.count()), std::memory_order_relaxed);

        // 시간이 초과된경우 업무 분산
        if (elapsed > interval)
        {
            SplitWorker(context);
        }
        
        auto sleep_time = interval - elapsed;
        if (sleep_time > milliseconds(0))
            std::this_thread::sleep_for(sleep_time);
    }
}
