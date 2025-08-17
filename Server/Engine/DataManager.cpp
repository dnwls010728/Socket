#include "pch.h"
#include "DataManager.h"

#include <iostream>

#include "jdbc/cppconn/prepared_statement.h"
#include "MySQL/MySQLManager.h"
#include "yaml-cpp/yaml.h"

DataManager::DataManager() :
    mob_map_(),
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
            data.mob_id = mob.first.as<uint32_t>();
            mob_map_[data.mob_id] = data;
        }

        YAML::Node exp_data = YAML::LoadFile("Content\\Data\\ExpData.data");
        for (const auto& exp : exp_data["exp"])
        {
            uint32_t level = exp.first.as<uint32_t>();
            int32_t exp_value = exp.second.as<int32_t>();
            exp_table_[level] = exp_value;
        }
        
        YAML::Node item_data = YAML::LoadFile("Content\\Data\\ItemData.data");
        for (const auto& item : item_data["items"])
        {
            ItemData data = item.second.as<ItemData>();
            data.item_id = item.first.as<uint32_t>();
            item_map_[data.item_id] = data;
        }
    }
    catch (const YAML::BadFile& e)
    {
        std::cout << e.what() << std::endl;
    }
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
                drop_data.item_id = result->getUInt("item_id");
                drop_data.min_count = result->getInt("min_count");
                drop_data.max_count = result->getInt("max_count");
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
