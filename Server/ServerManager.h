#pragma once

#include <unordered_map>
#include <mutex>
#include "TCPServerSocket.h"
#include "ClientInfo.h"
#include "RoomManager.h"
#include "CommonObject.h"
#include "SessionManager.h"
#include "MySQL/MySQLManager.h"

class ServerManager
{
public:
	ServerManager();

	bool Execute();

	bool OnClientConnected(const Net::TCPConnectionState& state);
	void OnClientDisconnected(const Net::TCPConnectionState& state);
	void OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet);
	void OnRoomListChanged(Room changed_room, RoomListUpdateType type);

private:
	Net::TCP::TCPServerSocket server_socket_;

	std::mutex client_map_mutex_;
	std::unordered_map<int, ClientInfo> client_map_;

	MySQLManager mysql_manager_;
	SessionManager session_manager_;
	RoomManager room_manager_;
};
