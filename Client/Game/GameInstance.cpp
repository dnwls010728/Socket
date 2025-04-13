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
    CSVReader::Parse(L"Data\\PropData.csv", prop_data_);

    // GameInstanceSubsystem을 상속받는 모든 클래스를 수집하고, 인스턴스를 생성한다.
    for (auto& t : rttr::type::get<GameInstanceSubsystem>().get_derived_classes())
    {
        if (t.is_valid() && t.is_class())
        {
            auto instance = t.create();
            if (instance.is_valid())
            {
                std::shared_ptr<GameInstanceSubsystem> subsystem = instance.get_value<std::shared_ptr<GameInstanceSubsystem>>();
                if (subsystem)
                {
                    subsystem->Init();
                    subsystems_[t.get_id()] = subsystem;
                }
            }
        }
    }
    
}

void GameInstance::Shutdown()
{
    auto it = subsystems_.begin();
    while (it != subsystems_.end())
    {
        auto subsystem = it->second;
        if (subsystem) subsystem->Deinit();
        it = subsystems_.erase(it);
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
