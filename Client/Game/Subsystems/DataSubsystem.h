#pragma once
#include "Misc/StringHelper.h"
#include "Subsystem/GameInstanceSubsystem.h"

#include "yaml-cpp/yaml.h"

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

class DataSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(DataSubsystem, GameInstanceSubsystem)
    
public:
    DataSubsystem();
    virtual ~DataSubsystem() override = default;

    virtual void Init() override;

    const MobData* GetMob(uint32_t id) const;
    const ItemData* GetItem(uint32_t id) const;
    
    int32_t GetExp(int32_t level) const;

    static DataSubsystem* Get();

private:
    std::unordered_map<uint32_t, MobData> mob_map_;
    std::unordered_map<uint32_t, ItemData> item_map_;
    
    std::array<int32_t, 51> exp_table_;
    
};
