#pragma once

#include "Data/PropData.h"

class GameInstanceSubsystem;

class GameInstance : public Singleton<GameInstance>
{
public:
    GameInstance();
    virtual ~GameInstance() override = default;

    void Init();
    void Shutdown();

    const PropData* GetPropData(int id);

    template <typename T>
    std::shared_ptr<T> GetSubsystem();

private:
    std::unordered_map<rttr::type::type_id, std::shared_ptr<GameInstanceSubsystem>> subsystems_;
    
    std::vector<PropData> prop_data_;
    
};

template <typename T>
std::shared_ptr<T> GameInstance::GetSubsystem()
{
    rttr::type t = rttr::type::get<T>();
    if (!t.is_valid()) return nullptr;

    auto it = subsystems_.find(t.get_id());
    if (it != subsystems_.end())
    {
        return std::dynamic_pointer_cast<T>(it->second);
    }

    return nullptr;
}
