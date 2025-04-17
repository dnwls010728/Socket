#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>

class Player;

class SessionManager
{
public:
    SessionManager();

    void AddSession(uint32_t unique_id, const std::shared_ptr<Player>& session);
    void RemoveSession(uint32_t unique_id);
    
    bool HasSession(uint32_t unique_id);

private:
    std::mutex mutex_;
    std::unordered_map<uint32_t, std::shared_ptr<Player>> sessions_;
    
};
