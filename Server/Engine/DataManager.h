#pragma once
#include "Singleton.h"
#include "Helper/StringHelper.h"
#include "yaml-cpp/node/convert.h"

struct MobStats
{
    int32_t lv;
    float hp;
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

class DataManager : public Singleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager() override = default;

    void Init();

private:
    std::unordered_map<uint32_t, MobData> mob_data_map;
    
};
