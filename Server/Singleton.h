#pragma once
#include <mutex>

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* Get()
    {
        if (!instance_) instance_ = std::make_unique<T>();
        return instance_.get();
    }

    virtual void Release()
    {
        instance_.reset();
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    
private:
    static std::unique_ptr<T> instance_;
    
};

template <typename T>
std::unique_ptr<T> Singleton<T>::instance_ = nullptr;