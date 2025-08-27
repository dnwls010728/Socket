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
    int32_t exp;
    float speed;
};

struct MobData
{
    uint32_t mob_id;
    MobStats stats;
    std::wstring animation_pack;
};

struct ItemStatData
{
};

struct ItemEffectData
{
    int32_t hp;
    int32_t hp_percent;
    int32_t atk;
    int32_t def;
    int32_t dig;
    int32_t duration;
    int32_t cooldown;
};

struct ItemData
{
    uint32_t item_id;

    std::wstring name;
    std::wstring desc;

    int32_t price;
    int32_t max_count;

    union
    {
        ItemStatData stat;
        ItemEffectData effect;
    };
};

struct MobDropData
{
    uint32_t item_id;

    int32_t min_count;
    int32_t max_count;
    int32_t chance;
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
            data.exp = node["exp"].as<int32_t>(0);
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

    template<>
    struct convert<ItemStatData>
    {
        static bool decode(const Node& node, ItemStatData& data)
        {
            if (!node.IsMap()) return false;
            return true;
        }
    };

    template<>
    struct convert<ItemEffectData>
    {
        static bool decode(const Node& node, ItemEffectData& data)
        {
            if (!node.IsMap()) return false;
            data.hp = node["hp"].as<int32_t>(0);
            data.hp_percent = node["hp_percent"].as<int32_t>(0);
            data.atk = node["atk"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.dig = node["dig"].as<int32_t>(0);
            data.duration = node["duration"].as<int32_t>(0);
            data.cooldown = node["cooldown"].as<int32_t>(0);
            return true;
        }
    };
    
    template<>
    struct convert<ItemData>
    {
        static bool decode(const Node& node, ItemData& data)
        {
            if (!node.IsMap()) return false;
            data.item_id = node["id"].as<uint32_t>(0);
            data.name = StringHelper::UTF8ToUTF16(node["name"].as<std::string>(""));
            data.desc = StringHelper::UTF8ToUTF16(node["desc"].as<std::string>(""));
            data.price = node["price"].as<int32_t>(0);
            data.max_count = node["max_count"].as<int32_t>(0);
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

    const MobData* GetMob(uint32_t id) const;
    const ItemData* GetItem(uint32_t id) const;
    
    const std::vector<MobDropData>* GetDrop(uint32_t id);
    
    int32_t GetExp(uint32_t level) const;

private:
    // YAML
    std::unordered_map<uint32_t, MobData> mob_map_;
    std::unordered_map<uint32_t, ItemData> item_map_;

    // Database
    std::unordered_map<uint32_t, std::vector<MobDropData>> mob_drop_map_;

    std::array<int32_t, 51> exp_table_;
    
};
