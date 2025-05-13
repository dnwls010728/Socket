#include "pch.h"
#include "DataSubsystem.h"

#include "Data/ItemData.h"
#include "Misc/StringHelper.h"

DataSubsystem::DataSubsystem() :
    data_map_(),
    item_map_()
{
}

void DataSubsystem::Init()
{
    GameInstanceSubsystem::Init();

    LoadMeta(DataType::kItem, L"Data\\Item.data");

}

const YAML::Node& DataSubsystem::GetData(DataType data_type)
{
    auto it = data_map_.find(data_type);
    if (it != data_map_.end()) return it->second;
    return YAML::Node();
}

ItemData* DataSubsystem::GetItem(int32_t item_id)
{
    auto [it, inserted] = item_map_.emplace(item_id, std::make_unique<ItemData>(item_id));
    return it->second.get();
}

bool DataSubsystem::LoadMeta(DataType data_type, const std::wstring& file_path)
{
    std::string path = StringHelper::UTF16ToUTF8(L".\\Content\\" + file_path);

    try
    {
        YAML::Node data = YAML::LoadFile(path);
        data_map_[data_type] = data;
        return true;
    }
    catch (const YAML::BadFile& e)
    {
        Logger::Print(L"Failed to load file: %s", file_path.c_str());
        Logger::Print(L"Error: %s", StringHelper::UTF8ToUTF16(e.what()).c_str());
        return false;
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
