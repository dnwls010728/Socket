#pragma once
#include "NetTCPSocket.h"
#include "TCPClientSocket.h"
#include "Subsystems/GameInstanceSubsystem.h"

class SessionSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(SessionSubsystem, GameInstanceSubsystem)
    
public:
    SessionSubsystem();
    virtual ~SessionSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;
    
};
