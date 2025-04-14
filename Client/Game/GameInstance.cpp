#include "pch.h"
#include "GameInstance.h"

#include "GameInstanceSubsystem.h"
#include "Data/CSVReader.h"

GameInstance::GameInstance() :
    subsystems_(),
    prop_data_()
{
}

void GameInstance::Init()
{
    CollectSubsystems();
    
    CSVReader::Parse(L"Data\\PropData.csv", prop_data_);
    
}

void GameInstance::Shutdown()
{
    auto it = subsystems_.begin();
    for (; it != subsystems_.end(); ++it)
    {
        if (it->second) it->second->Deinit();
    }

    subsystems_.clear();
}

void GameInstance::CollectSubsystems()
{
    for (auto& t : rttr::type::get<GameInstanceSubsystem>().get_derived_classes())
    {
        if (t.is_valid() && t.is_class())
        {
            auto instance = t.create();
            if (instance.is_valid())
            {
                GameInstanceSubsystem* subsystem = instance.get_value<GameInstanceSubsystem*>();
                if (subsystem)
                {
                    subsystem->Init();
                    subsystems_[t.get_id()] = std::unique_ptr<GameInstanceSubsystem>(subsystem);
                }
            }
        }
    }
}

const PropData* GameInstance::GetPropData(int id)
{
    auto it = std::ranges::find_if(prop_data_, [id](const PropData& data) {
        return data.id == id;
    });

    if (it != prop_data_.end()) return &(*it);
    return nullptr;
}
