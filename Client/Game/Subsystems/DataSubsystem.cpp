#include "pch.h"
#include "DataSubsystem.h"

#include "GameInstance.h"

DataSubsystem::DataSubsystem() :
    mob_map_(),
    exp_table_()
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
            mob_map_[data.mob_id] = data;
        }
        
        YAML::Node exp_data = YAML::LoadFile("Content\\Data\\ExpData.data");
        for (const auto& exp : exp_data["exp"])
        {
            int32_t level = exp.first.as<int32_t>();
            int32_t exp_value = exp.second.as<int32_t>();
            exp_table_[level] = exp_value;
        }

        YAML::Node item_data = YAML::LoadFile("Content\\Data\\ItemData.data");
        for (const auto& item : item_data["items"])
        {
            ItemData data = item.second.as<ItemData>();
            data.item_id = item.first.as<uint32_t>();
            item_map_[data.item_id] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
    
}

const MobData* DataSubsystem::GetMob(uint32_t id) const
{
    auto it = mob_map_.find(id);
    if (it == mob_map_.end()) return nullptr;
    return &it->second;
}

const ItemData* DataSubsystem::GetItem(uint32_t id) const
{
    auto it = item_map_.find(id);
    if (it == item_map_.end()) return nullptr;
    return &it->second;
}

int32_t DataSubsystem::GetExp(int32_t level) const
{
    if (level >= exp_table_.size()) return 0;
    return exp_table_[level];
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
