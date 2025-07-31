#include "pch.h"
#include "DataManager.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

DataManager::DataManager() :
    mob_data_map_(),
    exp_table_()
{
}

void DataManager::Init()
{
    try
    {
        YAML::Node mob_data = YAML::LoadFile("Content\\Data\\MobData.data");
        for (const auto& mob : mob_data["mobs"])
        {
            MobData data = mob.second.as<MobData>();
            data.mob_id = mob.first.as<uint32_t>();
            mob_data_map_[data.mob_id] = data;
        }

        YAML::Node exp_data = YAML::LoadFile("Content\\Data\\ExpData.data");
        for (const auto& exp : exp_data["exp"])
        {
            uint32_t level = exp.first.as<uint32_t>();
            int32_t exp_value = exp.second.as<int32_t>();
            exp_table_[level] = exp_value;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
}

const MobData* DataManager::GetMobData(uint32_t id) const
{
    auto it = mob_data_map_.find(id);
    if (it == mob_data_map_.end()) return nullptr;
    return &it->second;
}

int32_t DataManager::GetExp(uint32_t level) const
{
    if (level >= exp_table_.size()) return 0;
    return exp_table_[level];
}
