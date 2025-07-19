#pragma once

#include "Engine/Singleton.h"
#include "TCPServerSocket.h"
#include "Engine/Map/World.h"
#include "MySQL/MySQLManager.h"

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager();
	virtual ~ServerManager() override = default;

	bool Execute();
	bool OnClientConnected(const Net::TCPConnectionState& state);
	bool DisconnectClient(int client_id);
	
	void OnClientDisconnected(const Net::TCPConnectionState& state);
	void OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet);
	void SendPacket(uint32_t client_id, const Net::IPacket& packet);

private:
	void CommandHandlerInitialize();
	
	std::unordered_map<std::wstring, std::function<void(const std::vector<std::wstring>&)>> command_handler_;
	
	Net::TCP::TCPServerSocket server_socket_;
};
