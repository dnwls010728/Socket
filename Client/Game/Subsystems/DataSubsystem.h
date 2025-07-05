#pragma once
#include "Misc/StringHelper.h"
#include "Subsystem/GameInstanceSubsystem.h"

#include "yaml-cpp/yaml.h"

struct MobStats
{
    int32_t lv;
    int32_t hp;
    float speed;
};

struct MobData
{
    MobStats stats;
    std::wstring animation_pack;
};

namespace YAML
{
    template<>
    struct convert<MobStats>
    {
        static bool decode(const Node& node, MobStats& data)
        {
            if (!node.IsMap()) return false;
            data.lv = node["lv"].as<int32_t>(0);
            data.hp = node["hp"].as<float>(0.f);
            data.speed = node["speed"].as<float>(0.f);
            return true;
        }
    };
    
    template<>
    struct convert<MobData>
    {
        static bool decode(const Node& node, MobData& data)
        {
            if (!node.IsMap()) return false;
            data.stats = node["stats"].as<MobStats>();
            data.animation_pack = StringHelper::UTF8ToUTF16(node["animation_pack"].as<std::string>(""));
            return true;
        }
    };
}

class DataSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(DataSubsystem, GameInstanceSubsystem)
    
public:
    DataSubsystem();
    virtual ~DataSubsystem() override = default;

    virtual void Init() override;

    const MobData* GetMobData(uint32_t id) const;

private:
    std::unordered_map<uint32_t, MobData> mob_data_map;
    
};
