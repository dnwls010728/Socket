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
            mob_data_map[mob.first.as<uint32_t>()] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
}
