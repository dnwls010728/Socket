#pragma once
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
    T* LoadAsync(const std::wstring& kPath, Function<void(T*)> callback);

private:
    std::unordered_map<std::wstring, std::unique_ptr<Asset>> assets_;
    
};

template <std::derived_from<Asset> T>
T* AssetManager::Load(const std::wstring& kPath)
{
    std::wstring path = L".\\Content\\" + kPath;
    if (assets_.contains(path))
    {
        Asset* asset = assets_[path].get();
        
        rttr::type type = rttr::type::get<T>();
        if (type == asset->get_type()) return static_cast<T*>(asset);
        return nullptr;
    }

    std::unique_ptr<T> asset = std::make_unique<T>();
    if (!asset->Load(path)) return nullptr;

    assets_[path] = std::move(asset);
    return static_cast<T*>(assets_[path].get());
}

template <std::derived_from<Asset> T>
T* AssetManager::LoadAsync(const std::wstring& kPath, Function<void(T*)> callback)
{
}
