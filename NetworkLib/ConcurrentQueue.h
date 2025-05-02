#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue() = default;
    ~ConcurrentQueue() = default;

    // 데이터를 큐에 복사하여 추가
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        cond_.notify_one();  // 대기 중인 스레드 하나를 깨움
    }

    // 데이터를 큐에 이동(move semantics)하여 추가
    void push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(item));
        }
        cond_.notify_one();
    }

    // 데이터를 큐가 비어있지 않을 때까지 대기하고 꺼냄
    // (데이터가 반드시 들어올 때까지 블록)
    void pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        item = std::move(queue_.front());
        queue_.pop();
    }

    // 지정한 시간(timeout) 동안 대기하여 데이터를 꺼내려고 시도
    // timeout 동안 데이터가 들어오지 않으면 false를 반환
    template<typename Rep, typename Period>
    bool pop_for(T& item, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!cond_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            return false;  // timeout 발생
        }
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // 즉시 데이터를 꺼내려고 시도, 성공하면 true, 큐가 비었으면 false 반환
    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // 큐가 비었는지 확인
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_;
};

#endif // CONCURRENT_QUEUE_H
