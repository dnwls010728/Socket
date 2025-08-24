#include "pch.h"
#include "PartyManager.h"
#include "Party.h"
#include "Player.h"
#include "MySQL/MySQLManager.h"
#include "Helper/StringHelper.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "jdbc/cppconn/resultset.h"

PartyManager::PartyManager():
    next_party_id_(1)
{
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (connection)
    {
        try
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT MAX(party_id) FROM character_info"));
            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            if (result->next())
            {
                next_party_id_ = result->getUInt(1) + 1;
                if (next_party_id_ == 0) next_party_id_ = 1;
            }
        }
        catch (sql::SQLException&)
        {
        }
    }
}

std::shared_ptr<Party> PartyManager::CreateParty(const std::wstring& party_name)
{
    std::lock_guard<std::mutex> lock(mtx_);

    const std::uint32_t party_id = AllocatePatyID();

    auto party = std::make_shared<Party>(party_id);
    if (!party_name.empty())
        party->SetPartyName(party_name);

    parties_.emplace(party_id, party);
    return party;
}

void PartyManager::DestroyParty(std::uint32_t party_id)
{
    std::lock_guard<std::mutex> lock(mtx_);
    parties_.erase(party_id);
}

std::shared_ptr<Party> PartyManager::GetParty(std::uint32_t party_id) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = parties_.find(party_id);
    if (it == parties_.end())
        return nullptr;
    return it->second;
}

std::vector<std::shared_ptr<Party>> PartyManager::GetParties() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<std::shared_ptr<Party>> out;
    out.reserve(parties_.size());
    for (const auto& party : parties_)
        out.push_back(party.second);
    return out;
}

std::vector<std::uint32_t> PartyManager::GetPartyIds() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<std::uint32_t> ids;
    ids.reserve(parties_.size());
    for (const auto& kv : parties_)
        ids.push_back(kv.first);
    return ids;
}

int PartyManager::GetPartyCount() const
{
    std::lock_guard<std::mutex> lock(mtx_);
    return static_cast<int>(parties_.size());
}

void PartyManager::AddPlayerToParty(std::uint32_t party_id, const std::shared_ptr<Player>& player)
{
    if (!player)
        return;

    auto party = GetOrCreatePartyFromDB(party_id);
    if (!party)
        return;

    party->AddPlayer(player);
}

void PartyManager::RemovePlayerFromParty(std::uint32_t party_id, std::uint32_t character_id)
{
    std::shared_ptr<Party> party;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = parties_.find(party_id);
        if (it == parties_.end())
            return;
        party = it->second;
    }
    if (party)
        party->RemovePlayer(character_id);
}

void PartyManager::SendPacket(std::uint32_t party_id, const Net::IPacket& packet, uint32_t exclusion_member)
{
    std::shared_ptr<Party> party;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = parties_.find(party_id);
        if (it == parties_.end()) return;
        party = it->second;
    }

    party->SendPacket(packet, exclusion_member);
}

std::shared_ptr<Party> PartyManager::GetOrCreatePartyFromDB(std::uint32_t party_id)
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = parties_.find(party_id);
        if (it != parties_.end())
            return it->second;
    }

    auto party = std::make_shared<Party>(party_id);

    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (connection)
    {
        try
        {
            std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT character_id, name, lv, hp, max_hp FROM character_info WHERE party_id = ?"));
            statement->setUInt(1, party_id);
            std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
            while (result->next())
            {
                PartyMemberInfo info;
                info.character_id = result->getUInt("character_id");
                info.name = StringHelper::UTF8ToUTF16(result->getString("name"));
                info.lv = result->getInt("lv");
                info.hp = result->getInt("hp");
                info.max_hp = result->getInt("max_hp");
                info.is_online = false;
                party->AddOfflineMember(info);
            }
        }
        catch (sql::SQLException&)
        {
        }
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        parties_.emplace(party_id, party);
    }
    return party;
}

uint32_t PartyManager::AllocatePatyID()
{
    std::uint32_t id = next_party_id_;
    auto addNextPartyID = [this]() {
        ++next_party_id_;
        if (next_party_id_ == 0)
            next_party_id_ = 1;
    };

    addNextPartyID();
    while (parties_.find(id) != parties_.end()) {
        id = next_party_id_;
        addNextPartyID();
    }
    return id;
}
