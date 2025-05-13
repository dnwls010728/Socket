#include "pch.h"
#include "GameInstance.h"

#include "Subsystem/GameInstanceSubsystem.h"

GameInstance::GameInstance() :
    subsystems_()
{
}

void GameInstance::Init()
{
    InitSubsystems();
}

void GameInstance::Shutdown()
{
    DeinitSubsystems();
}

void GameInstance::InitSubsystems()
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

void GameInstance::DeinitSubsystems()
{
    auto it = subsystems_.begin();
    for (; it != subsystems_.end(); ++it)
    {
        if (it->second) it->second->Deinit();
    }

    subsystems_.clear();
}
