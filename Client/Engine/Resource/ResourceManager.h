#pragma once
#include "Resource.h"
#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
    ResourceManager();
    virtual ~ResourceManager() override = default;

    template <std::derived_from<Resource> T>
    T* Load(const std::wstring& kPath);

private:
    std::unordered_map<std::wstring, std::unique_ptr<Resource>> resources_;
    
};

template <std::derived_from<Resource> T>
T* ResourceManager::Load(const std::wstring& kPath)
{
    std::wstring path = L".\\Content\\" + kPath;
    if (resources_.contains(path))
    {
        Resource* resource = resources_[path].get();
        
        rttr::type type = rttr::type::get<T>();
        rttr::type resource_type = rttr::type::get(*resource);

        if (type == resource_type) return static_cast<T*>(resource);
        return nullptr;
    }

    std::unique_ptr<T> resource = std::make_unique<T>();
    if (!resource->Load(path)) return nullptr;

    resources_[path] = std::move(resource);
    return static_cast<T*>(resources_[path].get());
}
