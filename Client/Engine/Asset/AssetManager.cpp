#include "pch.h"
#include "AssetManager.h"

AssetManager::AssetManager() : assets_()
{
}

void AssetManager::Tick()
{
    std::lock_guard lock(pending_mutex_);

    auto it = pending_loads_.begin();
    while (it != pending_loads_.end())
    {
        if (it->future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            std::unique_ptr<Asset> asset = it->future.get();
            Asset* raw_ptr = asset.get();

            if (asset)
            {
                std::lock_guard assets_lock(assets_mutex_);
                if (asset->get_type() == it->expected_type)
                    assets_[it->path] = std::move(asset);
            }

            for (auto& callback : it->callbacks)
            {
                callback(raw_ptr);
            }

            it = pending_loads_.erase(it);
        }
        else ++it;
    }
}
