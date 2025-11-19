#include "pch.h"
#include "DataSubsystem.h"

#include "GameInstance.h"

DataSubsystem::DataSubsystem() :
    mob_map_(),
    item_map_(),
    skill_map_(),
    projectile_map_(),
    exp_table_()
{
}

void DataSubsystem::Init()
{
    GameInstanceSubsystem::Init();
    
    try
    {
        YAML::Node mob_data = YAML::LoadFile("Content\\Data\\MobData.data");
        for (const auto& mob : mob_data["mobs"])
        {
            MobData data = mob.second.as<MobData>();
            data.id = mob.first.as<uint32_t>();
            mob_map_[data.id] = data;
        }

        YAML::Node item_data = YAML::LoadFile("Content\\Data\\ItemData.data");
        for (const auto& item : item_data["items"])
        {
            ItemData data = item.second.as<ItemData>();
            data.id = item.first.as<uint32_t>();
            
            uint32_t type = data.id / 100000;
            if (type == 1)
                data.stat = item.second["stat"].as<ItemStatData>();
            else if (type == 2)
                data.effect = item.second["effect"].as<ItemEffectData>();
            
            item_map_[data.id] = data;
        }
        
        YAML::Node skill_data = YAML::LoadFile("Content\\Data\\SkillData.data");
        for (const auto& skill : skill_data["skills"])
        {
            SkillData data = skill.second.as<SkillData>();
            data.id = skill.first.as<uint32_t>();
            skill_map_[data.id] = data;
        }

        YAML::Node projectile_data = YAML::LoadFile("Content\\Data\\ProjectileData.data");
        for (const auto& projectile : projectile_data["projectiles"])
        {
            ProjectileData data = projectile.second.as<ProjectileData>();
            data.id = projectile.first.as<uint32_t>();
            projectile_map_[data.id] = data;
        }
        
        YAML::Node exp_data = YAML::LoadFile("Content\\Data\\ExpData.data");
        for (const auto& exp : exp_data["exp"])
        {
            int32_t level = exp.first.as<int32_t>();
            int32_t exp_value = exp.second.as<int32_t>();
            exp_table_[level] = exp_value;
        }

        YAML::Node card_data = YAML::LoadFile("Content\\Data\\CardData.data");
        for (const auto& card : card_data["cards"])
        {
            CardData data = card.second.as<CardData>();
            data.id = card.first.as<uint32_t>();
            card_map_[data.id] = data;
        }

        YAML::Node npc_data = YAML::LoadFile("Content\\Data\\NPCData.data");
        for (const auto& npc : npc_data["npcs"])
        {
            NPCData data = npc.second.as<NPCData>();
            data.id = npc.first.as<uint32_t>();
            npc_map_[data.id] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
    
}

const MobData* DataSubsystem::GetMob(uint32_t id) const
{
    auto it = mob_map_.find(id);
    if (it == mob_map_.end()) return nullptr;
    return &it->second;
}

const ItemData* DataSubsystem::GetItem(uint32_t id) const
{
    auto it = item_map_.find(id);
    if (it == item_map_.end()) return nullptr;
    return &it->second;
}

const SkillData* DataSubsystem::GetSkill(uint32_t id) const
{
    auto it = skill_map_.find(id);
    if (it == skill_map_.end()) return nullptr;
    return &it->second;
}

const ProjectileData* DataSubsystem::GetProjectile(uint32_t id) const
{
    auto it = projectile_map_.find(id);
    if (it == projectile_map_.end()) return nullptr;
    return &it->second;
}

const CardData* DataSubsystem::GetCard(uint32_t id) const
{
    auto it = card_map_.find(id);
    if (it == card_map_.end()) return nullptr;
    return &it->second;
}

const NPCData* DataSubsystem::GetNPC(uint32_t id) const
{
    auto it = npc_map_.find(id);
    if (it == npc_map_.end()) return nullptr;
    return &it->second;
}

int32_t DataSubsystem::GetExp(int32_t level) const
{
    if (level >= exp_table_.size()) return 0;
    return exp_table_[level];
}

DataSubsystem* DataSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<DataSubsystem>();
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
