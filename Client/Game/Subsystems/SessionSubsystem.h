#pragma once
#include "NetTCPSocket.h"
#include "TCPClientSocket.h"
#include "Subsystems/GameInstanceSubsystem.h"

DECLARE_DELEGATE(OnPacketDelegate, const std::shared_ptr<Net::IPacket>&);

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

    FORCEINLINE bool IsLoggedIn() const { return is_logged_in_; }
    FORCEINLINE void SetLoggedIn(bool logged_in) { is_logged_in_ = logged_in; }

    FORCEINLINE Type::uint32 GetAccountUniqueID() const { return account_unique_id_; }
    FORCEINLINE void SetAccountUniqueID(Type::uint32 account_unique_id) { account_unique_id_ = account_unique_id; }
    
    OnPacketDelegate packet_handler;

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;

    bool is_logged_in_;
    
    Type::uint32 account_unique_id_;
    
};
