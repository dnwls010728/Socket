#pragma once
#include <mutex>
#include <unordered_map>

#include "Singleton.h"

class Session;

class SessionManager : public Singleton<SessionManager>
{
public:
    SessionManager();
    virtual ~SessionManager() override = default;

    void AddSession(std::unique_ptr<Session>& session);
    void RemoveSession(uint32_t client_id);

    Session* FindSessionByClientID(uint32_t client_id);
    Session* FindSessionByAccountID(uint32_t account_id);

    bool HasSessionByClientID(uint32_t client_id);
    bool HasSessionByAccountID(uint32_t account_id);

private:
    std::mutex mutex_;
    std::unordered_map<uint32_t, std::unique_ptr<Session>> sessions_;
    
};
