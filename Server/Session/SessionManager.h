#pragma once
#include <mutex>
#include <unordered_map>

class Session;

class SessionManager
{
public:
    SessionManager();

    void AddSession(const std::shared_ptr<Session>& session);
    void RemoveSession(int client_id);

    std::shared_ptr<Session> FindSessionByClientID(int client_id);
    std::shared_ptr<Session> FindSessionByAccountUniqueID(int account_unique_id);

    bool HasSessionByClientID(int client_id);
    bool HasSessionByAccountUniqueID(int account_unique_id);

private:
    std::mutex mutex_;
    std::unordered_map<int, std::shared_ptr<Session>> sessions_;
    
};
