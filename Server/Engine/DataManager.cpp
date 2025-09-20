#include "pch.h"
#include "DataManager.h"

#include <iostream>

#include "uuid.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "MySQL/MySQLManager.h"
#include "yaml-cpp/yaml.h"

DataManager::DataManager() :
    mob_map_(),
    item_map_(),
    skill_map_(),
    exp_table_(),
    mob_drop_map_()
{
}

void DataManager::Init()
{
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
        
        YAML::Node exp_data = YAML::LoadFile("Content\\Data\\ExpData.data");
        for (const auto& exp : exp_data["exp"])
        {
            uint32_t level = exp.first.as<uint32_t>();
            int32_t exp_value = exp.second.as<int32_t>();
            exp_table_[level] = exp_value;
        }

        YAML::Node card_data = YAML::LoadFile("Content\\Data\\CardData.data");
        for (const auto& card : card_data["cards"])
        {
            CardData data = card.second.as<CardData>();
            data.id = card.first.as<uint32_t>();
            card_map_[data.id] = data;
            card_ids_cache_.push_back(data.id);
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::ranges::generate(seed_data, std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen(generator);

    uuids::uuid new_uuid = gen();
    std::cout << "DataManager initialized. UUID: " << new_uuid << std::endl;
}

const MobData* DataManager::GetMob(uint32_t id) const
{
    auto it = mob_map_.find(id);
    if (it == mob_map_.end()) return nullptr;
    return &it->second;
}

const ItemData* DataManager::GetItem(uint32_t id) const
{
    auto it = item_map_.find(id);
    if (it == item_map_.end()) return nullptr;
    return &it->second;
}

const SkillData* DataManager::GetSkill(uint32_t id) const
{
    auto it = skill_map_.find(id);
    if (it == skill_map_.end()) return nullptr;
    return &it->second;
}

const CardData* DataManager::GetCard(uint32_t id) const
{
    auto it = card_map_.find(id);
    if (it == card_map_.end()) return nullptr;
    return &it->second;
}

const std::unordered_map<uint32_t, CardData>* DataManager::GetCards() const
{
    return &card_map_;
}

const std::vector<uint32_t>* DataManager::GetCardIDs() const
{
    return &card_ids_cache_;
}

const std::vector<MobDropData>* DataManager::GetDrop(uint32_t id)
{
    {
        auto it = mob_drop_map_.find(id);
        if (it != mob_drop_map_.end()) return &it->second;
    }

    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return nullptr;

    try
    {
        std::vector<MobDropData> drops;
        
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM drop_info WHERE dropper_id = ?"));
            statement->setUInt(1, id);

            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                MobDropData drop_data;
                drop_data.id = result->getUInt("item_id");
                drop_data.min_count = result->getInt("min_count");
                drop_data.max_count = result->getInt("max_count");
                drop_data.chance = result->getInt("chance");
                drops.emplace_back(drop_data);
            }
        }

        auto [it, _] = mob_drop_map_.insert_or_assign(id, std::move(drops));
        if (it != mob_drop_map_.end()) return &it->second;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return nullptr;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return nullptr;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return nullptr;
    }

    return nullptr;
}

int32_t DataManager::GetExp(uint32_t level) const
{
    if (level >= exp_table_.size()) return 0;
    return exp_table_[level];
}
