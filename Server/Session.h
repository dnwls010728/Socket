#pragma once
#include <cstdint>

class Session
{
public:
    Session(uint32_t client_id);
    ~Session() = default;

    inline uint32_t GetClientID() const { return client_id_; }

private:
    uint32_t client_id_;
    
};
