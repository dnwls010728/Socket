#include "pch.h"
#include "DataSubsystem.h"

#include "GameInstance.h"

DataSubsystem::DataSubsystem() :
    mob_data_map()
{
}

void DataSubsystem::Init()
{
    GameInstanceSubsystem::Init();
    
    try
    {
        YAML::Node mob_data = YAML::LoadFile("Content\\Data\\MobData.data");
        for (const auto& mob : mob_data["mobs"])
        {
            MobData data = mob.second.as<MobData>();
            data.mob_id = mob.first.as<uint32_t>();
            mob_data_map[data.mob_id] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
    
}

const MobData* DataSubsystem::GetMobData(uint32_t id) const
{
    auto it = mob_data_map.find(id);
    if (it == mob_data_map.end()) return nullptr;
    return &it->second;
}

DataSubsystem* DataSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<DataSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DataSubsystem>("DataSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
