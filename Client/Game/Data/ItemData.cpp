#include "pch.h"
#include "ItemData.h"

#include "GameInstance.h"
#include "Asset/AssetManager.h"
#include "Misc/StringHelper.h"
#include "Subsystems/DataSubsystem.h"
#include "Windows/DX/UITexture.h"
#include "yaml-cpp/yaml.h"

ItemData::ItemData(int32_t item_id) :
    item_id_(item_id),
    price_(0),
    max_stock_(0),
    category_(Category::kNone),
    name_(L""),
    desc_(L""),
    icon_(nullptr),
    is_valid_(false)
{
    const YAML::Node& item_data = GET_DATA()->GetData(DataSubsystem::DataType::kItem);
    if (item_data.IsNull()) return;

    if (item_data["items"].IsSequence())
    {
        for (const auto& item : item_data["items"])
        {
            if (item["id"].as<int32_t>() == item_id)
            {
                item_id_ = item["id"].as<int32_t>();

                category_ = static_cast<Category>(item["category"].as<uint8_t>());

                price_ = item["price"].as<int32_t>();
                max_stock_ = item["max_stock"].as<int32_t>();

                name_ = StringHelper::UTF8ToUTF16(item["name"].as<std::string>());
                desc_ = StringHelper::UTF8ToUTF16(item["desc"].as<std::string>());

                AssetManager* asset_manager = AssetManager::Get();
                icon_ = asset_manager->Load<UITexture>(StringHelper::UTF8ToUTF16(item["icon"].as<std::string>()));
                if (!icon_) icon_ = asset_manager->Load<UITexture>(L"UI\\Item\\Error.png");

                is_valid_ = true;
            }
        }
    }
}
