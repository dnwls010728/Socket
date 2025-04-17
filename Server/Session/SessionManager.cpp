#include "SessionManager.h"

#include "Session.h"

SessionManager::SessionManager() :
    mutex_(),
    sessions_()
{
}

void SessionManager::AddSession(const std::shared_ptr<Session>& session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_[session->GetClientID()] = session;
}

void SessionManager::RemoveSession(int client_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(client_id);
}

std::shared_ptr<Session> SessionManager::FindSessionByClientID(int client_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = sessions_.find(client_id);
    if (it != sessions_.end()) return it->second;

    return nullptr;
}

std::shared_ptr<Session> SessionManager::FindSessionByAccountID(int account_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& session : sessions_)
    {
        if (session.second->GetAccountUniqueID() == account_id)
            return session.second;
    }

    return nullptr;
}

bool SessionManager::HasSessionByClientID(int client_id)
{
    return FindSessionByClientID(client_id) != nullptr;
}

bool SessionManager::HasSessionByAccountID(int account_id)
{
    return FindSessionByAccountID(account_id) != nullptr;
}
