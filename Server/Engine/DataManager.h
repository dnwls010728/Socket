#pragma once
#include "Singleton.h"
#include "Helper/StringHelper.h"
#include "yaml-cpp/node/convert.h"

struct MobStats
{
    int32_t lv;
    int32_t hp;
    int32_t dmg;
    int32_t def;
    float speed;
};

struct MobDrops
{
    int32_t exp;
    int32_t color;
};

struct MobData
{
    uint32_t mob_id;
    MobStats stats;
    MobDrops drops;
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
            data.hp = node["hp"].as<int32_t>(0);
            data.dmg = node["dmg"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.speed = node["speed"].as<float>(0.f);
            return true;
        }
    };

    template<>
    struct convert<MobDrops>
    {
        static bool decode(const Node& node, MobDrops& data)
        {
            if (!node.IsMap()) return false;
            data.exp = node["exp"].as<int32_t>(0);
            data.color = node["color"].as<int32_t>(0);
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
            data.drops = node["drops"].as<MobDrops>();
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

    const MobData* GetMobData(uint32_t id) const;
    
    uint32_t GetExp(uint32_t level) const;

private:
    std::unordered_map<uint32_t, MobData> mob_data_map_;

    std::array<uint32_t, 51> exp_table_;
    
};
