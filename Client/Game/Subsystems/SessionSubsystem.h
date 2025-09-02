#pragma once
#include <CommonObject.h>

#include "IPacketHandler.h"
#include "NetTCPSocket.h"
#include "TCPClientSocket.h"
#include "Subsystem/GameInstanceSubsystem.h"

class OLD_Inventory;

enum class SessionState
{
    kNone,
    kConnected,
    kDisconnected,
    kLoggedIn,
    kInGame
};

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

    FORCEINLINE SessionState GetState() const { return state_; }
    FORCEINLINE void SetState(SessionState state) { state_ = state; }

    FORCEINLINE bool IsInGame() const { return state_ == SessionState::kInGame; }
    FORCEINLINE float GetServerTime() const { return client_socket_.GetServerTime(); }
    FORCEINLINE float GetClientTime() const { return Net::GetClientTime(); }
    
    OnPacketDelegate packet_handler;

    static SessionSubsystem* Get();

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;

    SessionState state_;

    std::unordered_map<uint16_t, std::unique_ptr<IPacketHandler>> handlers_;
    
};
