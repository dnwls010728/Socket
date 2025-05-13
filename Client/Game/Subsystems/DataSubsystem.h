#pragma once
#include "Subsystems/GameInstanceSubsystem.h"
#include "yaml-cpp/yaml.h"

#define GET_DATA() GameInstance::Get()->GetSubsystem<DataSubsystem>()

class ItemData;

class DataSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(DataSubsystem, GameInstanceSubsystem)
    
public:
    enum class DataType :uint8_t
    {
        kNone = 0,
        kItem
    };
    
    DataSubsystem();
    virtual ~DataSubsystem() override = default;

    virtual void Init() override;

    const YAML::Node& GetData(DataType data_type);

    ItemData* GetItem(int32_t item_id);

private:
    bool LoadMeta(DataType data_type, const std::wstring& file_path);
    
    std::unordered_map<DataType, YAML::Node> data_map_;

    std::map<int32_t, std::unique_ptr<ItemData>> item_map_;
    
};
