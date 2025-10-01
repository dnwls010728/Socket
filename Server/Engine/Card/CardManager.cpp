#include "pch.h"
#include "CardManager.h"

#include <CustomPacket.h>
#include <iomanip>
#include <random>
#include <sstream>

#include "DataManager.h"
#include "jdbc/cppconn/connection.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "Map/PlayerCharacter.h"
#include "MySQL/MySQLManager.h"

CardManager::CardManager(PlayerCharacter* player) :
    player_(player)
{
}

void CardManager::OnLevelUp()
{
    if (selecting_cards.empty())
    {
        CardGroup cards;
        if (CreateCards(cards))
        {
            SendSelectCardPacket(cards);
            for (const auto& card : cards)
                selecting_cards.push_back(card);
        }
    }
    else
    {
        CardGroup cards;
        CreatePendingCard(cards);
        pending_cards_.push(std::move(cards));
    }
}

void CardManager::OnCardSelected(const CardSelectInfo& card)
{
    if (selecting_cards.empty())
        return;

    std::string chosen_time = NowForDB();
    
    for (CardSelectInfo& c : selecting_cards)
    {
        StagedCard staged;
        staged.card_id     = c.card_id;
        staged.level       = c.level;
        staged.offer_id    = c.offer_id;
        staged.offered_at  = c.offered_at;
        staged.slot        = c.slot;
        staged.is_selected = (c.slot == card.slot && c.card_id == card.card_id);
        staged.chosen_at   = staged.is_selected ? chosen_time : "";
        staged_cards_.push_back(std::move(staged));
    }
    
    int new_level = card.level;
    auto it = owned_cards_.find(card.card_id);
    if (it == owned_cards_.end() || it->second < new_level)
        owned_cards_[card.card_id] = new_level;

    selecting_cards.clear();
    ComputeStats();
    
    if (!pending_cards_.empty())
    {
        CardGroup cards = pending_cards_.front();
        pending_cards_.pop();

        if (cards[0].offer_id.empty())
        {
            if (CreateCards(cards))
            {
                selecting_cards.assign(cards.begin(), cards.end());
                SendSelectCardPacket(cards);
            }
        }
        else
        {
            if (ActivatePendingCard(cards))
            {
                selecting_cards.assign(cards.begin(), cards.end());
                SendSelectCardPacket(cards);
            }
        }
        ComputeStats();
    }
}

void CardManager::OnLoadCharacter()
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection)
        return;

    try
    {
        LoadOwnedCards(connection);
        LoadOffers(connection);
        ComputeStats();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException in OnLoadCharacter: " << e.what() << std::endl;
    }
}

void CardManager::LoadOwnedCards(sql::Connection* connection)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        connection->prepareStatement(
            "SELECT level, card_id, chosen_at "
            "FROM v_character_card_choice "
            "WHERE character_id = ? ORDER BY chosen_at"
        )
    );
    stmt->setUInt(1, player_->GetObjectID());
    std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

    owned_cards_.clear();
    while (result->next())
    {
        int level   = result->getInt("level");
        int card_id = result->getInt("card_id");
        owned_cards_[card_id] = level;
    }
}

void CardManager::LoadOffers(sql::Connection* connection)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        connection->prepareStatement(
            "SELECT o.offer_id, o.level, o.status, o.offered_at, i.slot, i.card_id "
            "FROM card_offer_info o "
            "JOIN card_offer_item_info i ON o.offer_id = i.offer_id "
            "WHERE o.character_id = ? AND (o.status = 0 OR o.status = -1) "
            "ORDER BY o.offered_at"
        )
    );
    stmt->setUInt(1, player_->GetObjectID());
    std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

    selecting_cards.clear();
    while (!pending_cards_.empty()) pending_cards_.pop();

    std::unordered_map<std::string, CardGroup> groups;
    std::unordered_map<std::string, int> status_map;
    std::vector<std::string> order;

    while (result->next())
    {
        std::istream* blobStream = result->getBlob("offer_id");
        std::ostringstream oss;
        oss << blobStream->rdbuf();
        std::string offer_id = oss.str();

        int level      = result->getInt("level");
        int status     = result->getInt("status");
        std::string ts = result->getString("offered_at");
        int slot       = result->getInt("slot");
        int card_id    = result->getInt("card_id");

        if (!groups.contains(ts))
        {
            groups[ts] = {};
            order.push_back(ts);
        }
        status_map[ts] = status;

        CardSelectInfo info;
        info.offer_id = offer_id;
        info.level    = level;
        info.slot     = slot;
        info.card_id  = card_id;
        groups[ts][slot - 1] = info;
    }

    for (const auto& ts : order)
    {
        auto status_it = status_map.find(ts);
        if (status_it == status_map.end()) continue;
        const auto& g = groups[ts];
        if (status_it->second == 0)
            selecting_cards.assign(g.begin(), g.end());
        else if (status_it->second == -1)
            pending_cards_.push(g);
    }
}

void CardManager::OnUpdateDatabase()
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection)
        return;

    try
    {
        if (!selecting_cards.empty())
        {
            for (auto& c : selecting_cards)
            {
                {
                    std::unique_ptr<sql::PreparedStatement> stmt(
                        connection->prepareStatement(
                            "INSERT INTO card_offer_info (offer_id, character_id, level, status, offered_at) "
                            "VALUES (?, ?, ?, 0, ?) "
                            "ON DUPLICATE KEY UPDATE level=VALUES(level), status=0, offered_at=VALUES(offered_at)"
                        )
                    );
                    std::istringstream iss(std::string(c.offer_id.data(), c.offer_id.size()));
                    stmt->setBlob(1, &iss);
                    stmt->setUInt(2, player_->GetObjectID());
                    stmt->setUInt(3, c.level);
                    stmt->setString(4, c.offered_at);
                    stmt->executeUpdate();
                }
                {
                    std::unique_ptr<sql::PreparedStatement> stmt(
                        connection->prepareStatement(
                            "INSERT INTO card_offer_item_info (offer_id, slot, card_id) "
                            "VALUES (?, ?, ?) "
                            "ON DUPLICATE KEY UPDATE card_id=VALUES(card_id)"
                        )
                    );
                    std::istringstream iss(std::string(c.offer_id.data(), c.offer_id.size()));
                    stmt->setBlob(1, &iss);
                    stmt->setUInt(2, c.slot);
                    stmt->setUInt(3, c.card_id);
                    stmt->executeUpdate();
                }
            }
        }
        
        std::queue<CardGroup> temp_queue;
        while (!pending_cards_.empty())
        {
            CardGroup group = pending_cards_.front();
            pending_cards_.pop();

            for (int i = 0; i < 3; ++i)
            {
                if (group[i].offer_id.empty())
                {
                    std::string uuid;
                    if (!CreateUUID(uuid)) continue;
                    group[i].offer_id = uuid;
                }

                {
                    std::unique_ptr<sql::PreparedStatement> stmt(
                        connection->prepareStatement(
                            "INSERT INTO card_offer_info (offer_id, character_id, level, status, offered_at) "
                            "VALUES (?, ?, ?, -1, ?) "
                            "ON DUPLICATE KEY UPDATE level=VALUES(level), status=-1, offered_at=VALUES(offered_at)"
                        )
                    );
                    std::istringstream iss(std::string(group[i].offer_id.data(), group[i].offer_id.size()));
                    stmt->setBlob(1, &iss);
                    stmt->setUInt(2, player_->GetObjectID());
                    stmt->setUInt(3, group[i].level);
                    stmt->setString(4, group[i].offered_at);
                    stmt->executeUpdate();
                }
                {
                    std::unique_ptr<sql::PreparedStatement> stmt(
                        connection->prepareStatement(
                            "INSERT INTO card_offer_item_info (offer_id, slot, card_id) "
                            "VALUES (?, ?, ?) "
                            "ON DUPLICATE KEY UPDATE card_id=VALUES(card_id)"
                        )
                    );
                    std::istringstream iss(std::string(group[i].offer_id.data(), group[i].offer_id.size()));
                    stmt->setBlob(1, &iss);
                    stmt->setUInt(2, group[i].slot);
                    stmt->setUInt(3, group[i].card_id);
                    stmt->executeUpdate();
                }
            }
            temp_queue.push(std::move(group));
        }
        pending_cards_ = std::move(temp_queue);
        
        for (auto& sc : staged_cards_)
        {
            {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    connection->prepareStatement(
                        "INSERT INTO card_offer_info (offer_id, character_id, level, status, offered_at) "
                        "VALUES (?, ?, ?, 1, ?) "
                        "ON DUPLICATE KEY UPDATE status=1, offered_at=VALUES(offered_at)"
                    )
                );
                std::istringstream iss(std::string(sc.offer_id.data(), sc.offer_id.size()));
                stmt->setBlob(1, &iss);
                stmt->setUInt(2, player_->GetObjectID());
                stmt->setUInt(3, sc.level);
                stmt->setString(4, sc.offered_at);
                stmt->executeUpdate();
            }
            {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    connection->prepareStatement(
                        "INSERT INTO card_offer_item_info (offer_id, slot, card_id) "
                        "VALUES (?, ?, ?) "
                        "ON DUPLICATE KEY UPDATE card_id=VALUES(card_id)"
                    )
                );
                std::istringstream iss(std::string(sc.offer_id.data(), sc.offer_id.size()));
                stmt->setBlob(1, &iss);
                stmt->setUInt(2, sc.slot);
                stmt->setUInt(3, sc.card_id);
                stmt->executeUpdate();
            }
            if (sc.is_selected)
            {
                std::unique_ptr<sql::PreparedStatement> stmt(
                    connection->prepareStatement(
                        "INSERT INTO card_choice_info (offer_id, chosen_card_id, slot, chosen_at) "
                        "VALUES (?, ?, ?, ?)"
                    )
                );
                std::istringstream iss(std::string(sc.offer_id.data(), sc.offer_id.size()));
                stmt->setBlob(1, &iss);
                stmt->setUInt(2, sc.card_id);
                stmt->setUInt(3, sc.slot);
                stmt->setString(4, sc.chosen_at);
                stmt->executeUpdate();
            }
        }
        staged_cards_.clear();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException in OnUpdateDatabase: " << e.what() << std::endl;
    }
}

void CardManager::OnEnterMap()
{
    if (!selecting_cards.empty())
    {
        CardGroup cards;
        for (int i = 0; i < 3; ++i)
        {
            if (i < static_cast<int>(selecting_cards.size()))
                cards[i] = selecting_cards[i];
        }
        SendSelectCardPacket(cards);
        return;
    }

    if (!pending_cards_.empty())
    {
        CardGroup cards = pending_cards_.front();
        pending_cards_.pop();

        if (cards[0].offer_id.empty())
        {
            if (CreateCards(cards))
            {
                selecting_cards.assign(cards.begin(), cards.end());
                SendSelectCardPacket(cards);
            }
        }
        else
        {
            if (ActivatePendingCard(cards))
            {
                selecting_cards.assign(cards.begin(), cards.end());
                SendSelectCardPacket(cards);
            }
        }
        ComputeStats();
    }
}

const CardData* CardManager::GetRandomCardData() const
{
    const CardData* card_data = nullptr;
    auto cards_key_ptr = DataManager::Get()->GetCardIDs();
    
    size_t count = cards_key_ptr->size();
    if (count == 0)
        return nullptr;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, count - 1);

    int cnt = 20;
    while (cnt > 0)
    {
        int idx = dist(gen);
        auto card_id = cards_key_ptr->at(idx);
        card_data = DataManager::Get()->GetCard(card_id);
        if (card_data)
            break;
        cnt--;
    }
    return card_data;
}

bool CardManager::CreatePendingCard(CardGroup& cards) const
{
    for (int i = 0; i < 3; ++i)
    {
        cards[i].offer_id.clear();
        cards[i].level = 0;
        cards[i].card_id = 0;
        cards[i].slot = i + 1;
    }
    return true;
}

bool CardManager::ActivatePendingCard(CardGroup& group) const
{
    for (int i = 0; i < 3; ++i)
    {
        const CardData* card_data = GetRandomCardData();
        if (!card_data)
            return false;

        int prev_level = 0;
        auto it = owned_cards_.find(card_data->id);
        if (it != owned_cards_.end()) prev_level = it->second;

        group[i].card_id = card_data->id;
        group[i].level   = prev_level + 1;
        group[i].slot    = i + 1;
    }
    return true;
}

bool CardManager::CreateCards(CardGroup& cards) const
{
    for (int i = 0; i < 3; ++i)
    {
        const CardData* card_data = GetRandomCardData();
        if (!card_data)
            return false;

        std::string uuid;
        if (!CreateUUID(uuid))
            return false;

        int prev_level = 0;
        auto it = owned_cards_.find(card_data->id);
        if (it != owned_cards_.end()) prev_level = it->second;

        cards[i].offer_id = uuid;
        cards[i].level    = prev_level + 1;
        cards[i].card_id  = card_data->id;
        cards[i].slot     = i + 1;
    }
    return true;
}

bool CardManager::CreateUUID(std::string& uuid) const
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection)
        return false;

    try
    {
        std::unique_ptr<sql::PreparedStatement> stmt(
            connection->prepareStatement("SELECT UUID_TO_BIN(UUID()) AS id"));
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

        if (result->next())
        {
            std::istream* blobStream = result->getBlob("id");
            std::ostringstream oss;
            oss << blobStream->rdbuf();
            uuid = oss.str();
            return true;
        }
        return false;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException in CreateUUID: " << e.what() << std::endl;
        return false;
    }
}

std::string CardManager::NowForDB() const
{
    using namespace std::chrono;
    static std::atomic<int> counter{0};

    auto now = system_clock::now();
    auto itt = system_clock::to_time_t(now);

    std::tm tm;
    localtime_s(&tm, &itt);
    
    auto micros = duration_cast<microseconds>(now.time_since_epoch()) % 1000000;
    int offset = counter.fetch_add(1) % 1000;
    micros += microseconds(offset);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(6) << std::setfill('0') << micros.count();
    return oss.str();
}

void CardManager::SendSelectCardPacket(const CardGroup& cards) const
{
    std::vector<CardSelectInfo> cards_vec;
    cards_vec.reserve(3);
    for (int i = 0; i < 3; ++i)
        cards_vec.push_back(cards[i]);

    DoSelectCardPacket select_card_packet;
    select_card_packet.cards = std::move(cards_vec);
    player_->SendPacket(select_card_packet);
}

void CardManager::ComputeStats()
{
    effective_max_hp_ = 0;
    effective_atk_    = 0;
    effective_def_    = 0;
    effective_dig_    = 0;
    
    SkillManager& skill_manager = player_->GetSkillManager();
    for (auto& [card_id, level] : owned_cards_)
    {
        const CardData* data = DataManager::Get()->GetCard(card_id);
        if (!data) continue;
        effective_max_hp_ += data->max_hp * level;
        effective_atk_    += data->atk    * level;
        effective_def_    += data->def    * level;
        effective_dig_    += data->dig    * level;
        if (data->skill_id != 0)
            skill_manager.AddSkill(data->skill_id, level);
    }
}
