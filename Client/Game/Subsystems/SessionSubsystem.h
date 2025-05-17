#pragma once
#include <CommonObject.h>

#include "NetTCPSocket.h"
#include "TCPClientSocket.h"
#include "Subsystems/GameInstanceSubsystem.h"

#define GET_SESSION() \
    GameInstance::Get()->GetSubsystem<SessionSubsystem>()

class InventoryData;

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

    FORCEINLINE const CharacterInfo& GetCharacterInfo() const { return character_info_; }
    FORCEINLINE void SetCharacterInfo(const CharacterInfo& info) { character_info_ = info; }

    FORCEINLINE InventoryData* GetInventoryData() const { return inventory_data_.get(); }

    FORCEINLINE float GetServerTime() const { return client_socket_.GetServerTime(); }
    FORCEINLINE float GetClientTime() const { return Net::GetClientTime(); }
    
    OnPacketDelegate packet_handler;

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;

    SessionState state_;

    CharacterInfo character_info_;

    std::unique_ptr<InventoryData> inventory_data_;
    
};
