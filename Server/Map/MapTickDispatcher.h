#pragma once

#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <algorithm>

class MapBase;
class MapTickDispatcher
{
public:
    MapTickDispatcher();
    ~MapTickDispatcher();

    void Start(uint32_t tick_interval_ms, size_t max_maps_per_thread);
    void Stop();

    void AddMap(MapBase* map);
    void RemoveMap(MapBase* map);

    inline bool running() const { return running_.load(); }
private:
    struct WorkerContext
    {
        std::thread thread;
        std::vector<MapBase*> assigned_maps;
        std::mutex mutex;
        std::atomic<uint32_t> last_tick_duration_ms{ 0 };
    };

    void WorkerThread(WorkerContext* context, uint32_t tick_interval_ms);
    WorkerContext* FindBestWorker();
    void SplitWorker(WorkerContext* context);

    std::vector<std::unique_ptr<WorkerContext>> worker_contexts_;
    std::mutex worker_mutex_;

    std::atomic<bool> running_;
    uint32_t tick_interval_ms_;
    size_t max_maps_per_thread_;
};