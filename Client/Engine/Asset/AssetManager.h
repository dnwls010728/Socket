#pragma once
#include <future>

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

private:
    struct PendingLoad
    {
        std::wstring path;
        std::future<void*> future;
        Function<void(void*)> callback;
        rttr::type expected_type;
    };
    
    std::unordered_map<std::wstring, std::unique_ptr<Asset>> assets_;
    
};

template <std::derived_from<Asset> T>
T* AssetManager::Load(const std::wstring& kPath)
{
    std::wstring full_path = L".\\Content\\" + kPath;
    rttr::type type = rttr::type::get<T>();
    
    if (assets_.contains(full_path))
    {
        Asset* asset = assets_[full_path].get();
        
        if (type == asset->get_type())
            return static_cast<T*>(asset);
    }

    std::unique_ptr<T> asset = std::make_unique<T>();
    if (!asset->Load(full_path)) return nullptr;

    assets_[full_path] = std::move(asset);
    return static_cast<T*>(assets_[full_path].get());
}

template <std::derived_from<Asset> T>
void AssetManager::LoadAsync(const std::wstring& kPath, Function<void(T*)> callback)
{
    std::wstring full_path = L".\\Content\\" + kPath;
    rttr::type type = rttr::type::get<T>();

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
