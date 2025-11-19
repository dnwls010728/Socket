#pragma once
#include <future>
#include <mutex>

#include "Asset.h"
#include "Singleton.h"

class AssetManager : public Singleton<AssetManager>
{
public:
    AssetManager();
    virtual ~AssetManager() override = default;

    template <std::derived_from<Asset> T>
    T* Load(const std::wstring& kPath);

    template <std::derived_from<Asset> T>
    void LoadAsync(const std::wstring& kPath, Function<void(T*)> callback);

    void Tick();

private:
    struct PendingLoad
    {
        std::wstring path;
        std::future<std::unique_ptr<Asset>> future;
        std::vector<Function<void(Asset*)>> callbacks;
        rttr::type expected_type;
    };

    std::vector<PendingLoad> pending_loads_;
    std::unordered_map<std::wstring, std::unique_ptr<Asset>> assets_;

    std::mutex assets_mutex_;
    std::mutex pending_mutex_;
};

template <std::derived_from<Asset> T>
T* AssetManager::Load(const std::wstring& kPath)
{
    std::wstring full_path = L".\\Content\\" + kPath;
    rttr::type type = rttr::type::get<T>();

    {
        std::lock_guard lock(assets_mutex_);
        if (assets_.contains(full_path))
        {
            Asset* asset = assets_[full_path].get();

            if (type == asset->get_type())
                return static_cast<T*>(asset);
        }
    }

    std::unique_ptr<T> asset = std::make_unique<T>();
    if (!asset->Load(full_path)) return nullptr;

    {
        std::lock_guard lock(assets_mutex_);
        assets_[full_path] = std::move(asset);
        return static_cast<T*>(assets_[full_path].get());
    }
}

template <std::derived_from<Asset> T>
void AssetManager::LoadAsync(const std::wstring& kPath, Function<void(T*)> callback)
{
    std::wstring full_path = L".\\Content\\" + kPath;
    rttr::type type = rttr::type::get<T>();

    {
        std::lock_guard lock(assets_mutex_);
        if (assets_.contains(full_path))
        {
            Asset* asset = assets_[full_path].get();
            if (type == asset->get_type())
            {
                callback(static_cast<T*>(asset));
                return;
            }
        }
    }

    {
        std::lock_guard lock(pending_mutex_);
        for (auto& pending : pending_loads_)
        {
            if (pending.path == full_path && pending.expected_type == type)
            {
                pending.callbacks.push_back([callback](Asset* asset) {
                    callback(static_cast<T*>(asset));
                });
                return;
            }
        }
    }

    auto future = std::async(std::launch::async, [full_path]() -> std::unique_ptr<Asset>
    {
        std::unique_ptr<T> asset = std::make_unique<T>();
        if (!asset->Load(full_path))
            return nullptr;
        return asset;
    });

    std::lock_guard lock(pending_mutex_);
    pending_loads_.push_back({
        .path = full_path,
        .future = std::move(future),
        .callbacks = {[callback](Asset* asset) {
            callback(static_cast<T*>(asset));
        }},
        .expected_type = type
    });
}
