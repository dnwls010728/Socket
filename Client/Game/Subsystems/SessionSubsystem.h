#pragma once
#include "NetTCPSocket.h"
#include "TCPClientSocket.h"
#include "Subsystems/GameInstanceSubsystem.h"

DECLARE_DELEGATE(OnPacketDelegate, std::shared_ptr<Net::IPacket>);

class SessionSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(SessionSubsystem, GameInstanceSubsystem)
    
public:
    SessionSubsystem();
    virtual ~SessionSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    
    void ProcessPackets();
    void SendPacket(Net::IPacket& packet);
    
    OnPacketDelegate packet_handler;

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;
    
};
