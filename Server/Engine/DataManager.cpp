#include "pch.h"
#include "DataManager.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

DataManager::DataManager()
{
}

void DataManager::Init()
{
    try
    {
        YAML::Node mob_data = YAML::LoadFile("Content\\Data\\MobData.yaml");
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
}
