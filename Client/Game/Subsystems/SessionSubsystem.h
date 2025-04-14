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
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(std::shared_ptr<Net::IPacket>)>, std::decay_t<F>>>>
    void OnPacketReceived(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnPacketReceived(M* target, bool(M::*func)(std::shared_ptr<Net::IPacket>));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnPacketReceived(M* target, bool(M::*func)(std::shared_ptr<Net::IPacket>) const);

    void OnPacketReceived(bool(*func)(std::shared_ptr<Net::IPacket>));
    void ProcessPackets();

private:
    bool Connect(const Net::NetAddress& address);

    void Disconnect();
    
    Net::TCP::TCPClientSocket client_socket_;

    Function<bool(std::shared_ptr<Net::IPacket>)> packet_received_event_;
    
};

template <typename F, typename>
void SessionSubsystem::OnPacketReceived(F&& func)
{
    packet_received_event_ = std::forward<F>(func);
}

template <typename M, typename>
void SessionSubsystem::OnPacketReceived(M* target, bool(M::* func)(std::shared_ptr<Net::IPacket>))
{
    packet_received_event_ = {target, func};
}

template <typename M, typename>
void SessionSubsystem::OnPacketReceived(M* target, bool(M::* func)(std::shared_ptr<Net::IPacket>) const)
{
    packet_received_event_ = {target, func};
}
