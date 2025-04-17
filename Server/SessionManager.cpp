#include "SessionManager.h"

#include "Session.h"

SessionManager::SessionManager() :
    mutex_(),
    sessions_()
{
}

void SessionManager::AddSession(uint32_t unique_id, const std::shared_ptr<Session>& session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.emplace(unique_id, session);
}

void SessionManager::RemoveSession(uint32_t unique_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(unique_id);
}

bool SessionManager::HasSession(uint32_t unique_id)
{
	std::lock_guard<std::mutex> lock(mutex_);
    auto it = sessions_.find(unique_id);
    if (it != sessions_.end()) return true;
    
    return false;
}
