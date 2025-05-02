#include "SessionManager.h"

#include "Session.h"

SessionManager::SessionManager() :
    mutex_(),
    sessions_()
{
}

void SessionManager::AddSession(std::unique_ptr<Session>& session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_[session->GetClientID()] = std::move(session);
}

void SessionManager::RemoveSession(uint32_t client_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(client_id);
}

Session* SessionManager::FindSessionByClientID(uint32_t client_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = sessions_.find(client_id);
    if (it != sessions_.end()) return it->second.get();

    return nullptr;
}

Session* SessionManager::FindSessionByAccountUniqueID(uint32_t account_unique_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& session : sessions_)
    {
        if (session.second->GetAccountUniqueID() == account_unique_id)
            return session.second.get();
    }

    return nullptr;
}

bool SessionManager::HasSessionByClientID(uint32_t client_id)
{
    return FindSessionByClientID(client_id) != nullptr;
}

bool SessionManager::HasSessionByAccountUniqueID(uint32_t account_unique_id)
{
    return FindSessionByAccountUniqueID(account_unique_id) != nullptr;
}
