#include "pch.h"
#include "DataManager.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

DataManager::DataManager() :
    mob_data_map()
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
            mob_data_map[data.mob_id] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
}

const MobData* DataManager::GetMobData(uint32_t id) const
{
    auto it = mob_data_map.find(id);
    if (it == mob_data_map.end()) return nullptr;
    return &it->second;
}
