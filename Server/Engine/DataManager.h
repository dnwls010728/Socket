#pragma once
#include "Singleton.h"
#include "Helper/StringHelper.h"
#include "Math/Bounds.h"
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
    uint32_t id;
    MobStats stats;
    std::wstring animation_pack;
};

struct ItemStatData
{
    int32_t req_lv;
    int32_t max_hp;
    int32_t atk;
    int32_t def;
    int32_t dig;
};

struct ItemEffectData
{
    int32_t hp;
    int32_t hp_percent;
    int32_t max_hp;
    int32_t atk;
    int32_t def;
    int32_t dig;
    int32_t duration;
    int32_t cooldown;

    wchar_t block_color[7]; // 6 characters + null terminator
};

struct ItemData
{
    uint32_t id;

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

struct HitFrame
{
    Bounds hitbox;

    int32_t damage;
    int32_t hit_count;
    int32_t max_targets;

    float time_offset;

    std::wstring hit_effect_pack;
    std::wstring hit_effect_animation;
};

struct SkillData
{
    uint32_t id;
    
    std::wstring name;
    std::wstring desc;
    std::wstring animation_pack;
    std::wstring animation;
    std::wstring sound;

    int32_t max_hp;
    int32_t atk;
    int32_t def;
    int32_t dig;
    int32_t duration;
    int32_t cooldown;

    uint32_t projectile_id;

    std::vector<HitFrame> hit_frames;
};

struct ProjectileData
{
    uint32_t id;

    Math::Vector2 size;
    float speed;
    float max_lifetime;

    int32_t damage;
    int32_t damage_count;
    int32_t max_targets;

    std::wstring animation;
    std::wstring animation_pack;

    std::wstring hit_effect_pack;
    std::wstring hit_effect_animation;
};

struct MobDropData
{
    uint32_t id;

    int32_t min_count;
    int32_t max_count;
    int32_t chance;
};

struct CardData
{
    uint32_t id;

    uint32_t skill_id;
    
    int32_t max_hp;
    int32_t atk;
    int32_t def;
    int32_t dig;
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
            data.req_lv = node["req_lv"].as<int32_t>(1);
            data.max_hp = node["max_hp"].as<int32_t>(0);
            data.atk = node["atk"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.dig = node["dig"].as<int32_t>(0);
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
            data.max_hp = node["max_hp"].as<int32_t>(0);
            data.atk = node["atk"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.dig = node["dig"].as<int32_t>(0);
            data.duration = node["duration"].as<int32_t>(0);
            data.cooldown = node["cooldown"].as<int32_t>(0);

            std::wstring block_color = StringHelper::UTF8ToUTF16(node["block_color"].as<std::string>(""));
            wcscpy_s(data.block_color, block_color.c_str());
            return true;
        }
    };
    
    template<>
    struct convert<ItemData>
    {
        static bool decode(const Node& node, ItemData& data)
        {
            if (!node.IsMap()) return false;
            data.name = StringHelper::UTF8ToUTF16(node["name"].as<std::string>(""));
            data.desc = StringHelper::UTF8ToUTF16(node["desc"].as<std::string>(""));
            data.price = node["price"].as<int32_t>(0);
            data.max_count = node["max_count"].as<int32_t>(0);
            return true;
        }
    };

    template<>
    struct convert<Math::Vector2>
    {
        static bool decode(const Node& node, Math::Vector2& data)
        {
            if (!node.IsSequence() || node.size() != 2) return false;
            data.x = node[0].as<float>(0.f);
            data.y = node[1].as<float>(0.f);
            return true;
        }
    };

    template<>
    struct convert<Bounds>
    {
        static bool decode(const Node& node, Bounds& data)
        {
            if (!node.IsMap()) return false;
            Math::Vector2 center = node["center"].as<Math::Vector2>();
            Math::Vector2 size = node["size"].as<Math::Vector2>();
            data = {center, size};
            return true;
        }
    };

    template<>
    struct convert<HitFrame>
    {
        static bool decode(const Node& node, HitFrame& data)
        {
            if (!node.IsMap()) return false;
            data.hitbox = node["hitbox"].as<Bounds>();
            data.damage = node["damage"].as<int32_t>(0);
            data.hit_count = node["hit_count"].as<int32_t>(0);
            data.max_targets = node["max_targets"].as<int32_t>(0);
            data.time_offset = node["time_offset"].as<float>(0.f);
            data.hit_effect_pack = StringHelper::UTF8ToUTF16(node["hit_effect_pack"].as<std::string>(""));
            data.hit_effect_animation = StringHelper::UTF8ToUTF16(node["hit_effect_animation"].as<std::string>(""));
            return true;
        }
    };

    template<>
    struct convert<SkillData>
    {
        static bool decode(const Node& node, SkillData& data)
        {
            if (!node.IsMap()) return false;
            data.name = StringHelper::UTF8ToUTF16(node["name"].as<std::string>(""));
            data.desc = StringHelper::UTF8ToUTF16(node["desc"].as<std::string>(""));
            data.animation_pack = StringHelper::UTF8ToUTF16(node["animation_pack"].as<std::string>(""));
            data.animation = StringHelper::UTF8ToUTF16(node["animation"].as<std::string>(""));
            data.sound = StringHelper::UTF8ToUTF16(node["sound"].as<std::string>(""));
            data.max_hp = node["max_hp"].as<int32_t>(0);
            data.atk = node["atk"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.dig = node["dig"].as<int32_t>(0);
            data.duration = node["duration"].as<int32_t>(0);
            data.cooldown = node["cooldown"].as<int32_t>(0);

            data.projectile_id = node["projectile"].as<uint32_t>(0);

            data.hit_frames.clear();
            if (auto frames = node["hit_frames"]; frames)
            {
                for (const auto& frame : frames)
                    data.hit_frames.push_back(frame.as<HitFrame>());
            }

            return true;
        }
    };

    template<>
    struct convert<ProjectileData>
    {
        static bool decode(const Node& node, ProjectileData& data)
        {
            if (!node.IsMap()) return false;

            data.size = Math::Vector2::One();
            if (auto size_node = node["size"]; size_node)
                data.size = size_node.as<Math::Vector2>();
            data.speed = node["speed"].as<float>(0.f);
            data.max_lifetime = node["max_lifetime"].as<float>(0.f);
            data.damage = node["damage"].as<int32_t>(0);
            data.damage_count = node["damage_count"].as<int32_t>(1);
            data.max_targets = node["max_targets"].as<int32_t>(0);
            data.animation = StringHelper::UTF8ToUTF16(node["animation"].as<std::string>("Idle"));
            data.animation_pack = StringHelper::UTF8ToUTF16(node["animation_pack"].as<std::string>(""));
            data.hit_effect_pack = StringHelper::UTF8ToUTF16(node["hit_effect_pack"].as<std::string>(""));
            data.hit_effect_animation = StringHelper::UTF8ToUTF16(node["hit_effect_animation"].as<std::string>(""));

            return true;
        }
    };

    template<>
    struct convert<CardData>
    {
        static bool decode(const Node& node, CardData& data)
        {
            if (!node.IsMap()) return false;
            data.skill_id = node["skill_id"].as<uint32_t>(0);
            data.max_hp = node["max_hp"].as<int32_t>(0);
            data.atk = node["atk"].as<int32_t>(0);
            data.def = node["def"].as<int32_t>(0);
            data.dig = node["dig"].as<int32_t>(0);
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
    const SkillData* GetSkill(uint32_t id) const;
    const ProjectileData* GetProjectile(uint32_t id) const;
    const CardData* GetCard(uint32_t id) const;
    const std::unordered_map<uint32_t, CardData>* GetCards() const;
    const std::vector<uint32_t>* GetCardIDs() const;
    
    const std::vector<MobDropData>* GetDrop(uint32_t id);
    
    int32_t GetExp(uint32_t level) const;

private:
    // YAML
    std::unordered_map<uint32_t, MobData> mob_map_;
    std::unordered_map<uint32_t, ItemData> item_map_;
    std::unordered_map<uint32_t, SkillData> skill_map_;
    std::unordered_map<uint32_t, ProjectileData> projectile_map_;
    std::unordered_map<uint32_t, CardData> card_map_;

    // Database
    std::unordered_map<uint32_t, std::vector<MobDropData>> mob_drop_map_;

    std::array<int32_t, 51> exp_table_;

    // cache
    std::vector<uint32_t> card_ids_cache_;
    
};
