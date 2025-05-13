#include "pch.h"
#include "DataSubsystem.h"

#include "Data/CSVReader.h"
#include "Misc/StringHelper.h"
#include "yaml-cpp/yaml.h"

DataSubsystem::DataSubsystem()
{
}

void DataSubsystem::Init()
{
    GameInstanceSubsystem::Init();

    try
    {
        YAML::Node yaml = YAML::LoadFile(".\\Content\\Data\\ItemData.meta");
        if (!yaml.IsNull())
        {
            if (yaml["items"].IsSequence())
            {
                for (const auto& item : yaml["items"])
                {
                    if (item.IsMap())
                    {
                        int32_t id = item["id"].as<int32_t>();
                        int32_t category = item["category"].as<int32_t>();
                        
                        std::wstring name = StringHelper::UTF8ToUTF16(item["name"].as<std::string>());
                        std::wstring desc = StringHelper::UTF8ToUTF16(item["desc"].as<std::string>());
                        std::wstring icon = StringHelper::UTF8ToUTF16(item["icon"].as<std::string>());
                        
                        int32_t price = item["price"].as<int32_t>();
                        int32_t max_stack = item["max_stack"].as<int32_t>();

                        Logger::Print(L"Item ID: %d, Category: %d, Name: %s, Description: %s, Icon: %s, Price: %d, Max Stack: %d",
                            id, category, name.c_str(), desc.c_str(), icon.c_str(), price, max_stack);
                    }
                }
            }
        }
    }
    catch (const YAML::BadFile& e)
    {
        Logger::Print(L"Failed to load YAML file: %s", e.what());
    }

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
