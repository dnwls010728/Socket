#include "ServerManager.h"
#include <string>
#include <iostream>
#include "NetworkManager.h"	
#include "CustomPacket.h"
#include "CustomSerializer.h"

ServerManager::ServerManager()
{
	server_socket_.SetSerializerFactory([]() { return std::make_unique<CustomSerializer>(); });

	// 콜벡 설정
	server_socket_.SetClientAcceptedCallback(std::bind(&ServerManager::OnClientConnected, this, std::placeholders::_1));
	server_socket_.SetClientDisconnectedCallback(std::bind(&ServerManager::OnClientDisconnected, this, std::placeholders::_1));
	server_socket_.SetPacketReceivedCallback(std::bind(&ServerManager::OnPacketReceived, this, std::placeholders::_1, std::placeholders::_2));

	room_manager_.SetRoomListUpdateCallback(std::bind(&ServerManager::OnRoomListChanged, this, std::placeholders::_1, std::placeholders::_2));
}

bool ServerManager::Execute()
{
	Net::WSAInit();

	Net::NetAddress server_address("0.0.0.0", 9000);
	if (server_socket_.Start(server_address, 0) == false)
	{
		return false;
	}
	
	// 명령어 입력 루프
	bool stop_flag = false;
	while (stop_flag == false)
	{
		std::string command;
		std::cin >> command;

		std::string first_token;
		size_t token_idx = command.find(' ',0);
		if (token_idx != std::string::npos)
		{
			first_token = command.substr(0, token_idx);
		}
		else
		{
			first_token = command;
		}

		if (first_token == "/exit")
		{
			stop_flag = true;
		}
		else if (first_token == "/help")
		{
			std::cout << "Available commands:" << std::endl;
			std::cout << "/exit - Exits the server." << std::endl;
		}
		else if(first_token == "/room")
		{
			// 임시
			room_manager_.CreateRoom(L"Test룸", 5);
		}
		else
		{
			std::cout << "Error: Unrecognized command. Please check '/help' for a list of available commands." << std::endl;
		}
		
	}

	server_socket_.Stop();
	Net::WSAUninit();
	return true;
}

bool ServerManager::OnClientConnected(const Net::TCPConnectionState& state)
{
	std::cout << "Client connected: " << state.address.ToString().c_str() << std::endl;
	{
		std::lock_guard<std::mutex> lock(client_map_mutex_);
		client_map_.emplace(state.uniqueKey, state.uniqueKey);
	}

	return true;
}

void ServerManager::OnClientDisconnected(const Net::TCPConnectionState& state)
{
	std::cout << "Client disconnected: " << state.address.ToString().c_str() << std::endl;
	{
		std::lock_guard<std::mutex> lock(client_map_mutex_);
		client_map_.erase(state.uniqueKey);
	}
}

void ServerManager::OnPacketReceived(const Net::TCPConnectionState& state, std::unique_ptr<Net::IPacket> packet)
{
	switch (packet->GetPacketID())
	{
	case MessagePacket::StaticPacketID:
	{
		MessagePacket* message_packet = static_cast<MessagePacket*>(packet.get());
		std::cout << "Message received from " << state.address.ToString().c_str() << ": " << message_packet->message.c_str() << std::endl;
		break;
	}
	
	case RoomListPacketReq::StaticPacketID:
	{
		RoomListPacketReq* room_list_req = static_cast<RoomListPacketReq*>(packet.get());
		RoomList room_list = room_manager_.GetRoomList();
		
		RoomListPacketAck room_list_ack;
		room_list_ack.sequence = room_list_req->sequence;
		room_list_ack.room_list = room_list;
		server_socket_.SendPacketToClient(state.uniqueKey, room_list_ack);
		break;
	}
	
	case CreateRoomPacketReq::StaticPacketID:
	{
		CreateRoomPacketReq* create_room_req = static_cast<CreateRoomPacketReq*>(packet.get());
		std::wstring room_title = create_room_req->room_title;
		int max_user_count = create_room_req->max_user_count;

		// 방 생성
		int room_number = room_manager_.CreateRoom(room_title, max_user_count);
		bool create_result = room_number != -1 ? true : false;
		if (create_result)
		{
			// 방장은 자동 입장
			bool join_room_result = room_manager_.EnterRoom(room_number, state.uniqueKey);
			if (join_room_result == false)
			{
				room_manager_.RemoveRoom(room_number);
				create_result = false;
				room_number = -1;
			}
		}

		CreateRoomPacketAck create_room_ack;
		create_room_ack.sequence = create_room_req->sequence;
		create_room_ack.result = create_result;
		create_room_ack.room_number = room_number;
		create_room_ack.room_title = room_title;
		create_room_ack.max_user_count = max_user_count;

		server_socket_.SendPacketToClient(state.uniqueKey, create_room_ack);
		break;
	}

	case RoomEnterPacketReq::StaticPacketID:
	{
		RoomEnterPacketReq* room_enter_req = static_cast<RoomEnterPacketReq*>(packet.get());
		int room_number = room_enter_req->room_number;

		bool join_room_result = room_manager_.EnterRoom(room_number, state.uniqueKey);

		RoomEnterPacketAck room_enter_ack;
		room_enter_ack.sequence = room_enter_req->sequence;
		room_enter_ack.result = join_room_result;

		if (join_room_result)
		{
			std::lock_guard<std::mutex> lock(client_map_mutex_);

			Room room;
			bool get_roomm = room_manager_.GetRoom(room_number, room);
			if (get_roomm == false)
				return;

			auto user_list = room.GetUserList();
			user_list.erase(state.uniqueKey);
			
			for (const int& uniqueKey : user_list)
			{
				auto find_result = client_map_.find(uniqueKey);
				if (find_result != client_map_.end())
				{
					ClientData client_data;
					client_data.client_name = find_result->second.GetClientName();
					client_data.client_number = find_result->second.GetClientNumber();

					room_enter_ack.room_info_ex.client_list.push_back(std::move(client_data));
				}
			}
			room_enter_ack.room_info_ex.room_number = room_number;
			room_enter_ack.room_info_ex.current_user_count = room.GetCurrentUserCount();
			room_enter_ack.room_info_ex.max_user_count = room.GetMaxUserCount();
			room_enter_ack.room_info_ex.room_title = room.GetRoomTitle();
		}

		server_socket_.SendPacketToClient(state.uniqueKey, room_enter_ack);
		break;
	}

	case RoomExitPacket::StaticPacketID:
	{
		RoomExitPacket* exit_packet = static_cast<RoomExitPacket*>(packet.get());
		int room_number = -1;
		
		std::unordered_map<int,ClientInfo>::iterator it;
		{
			std::lock_guard lock(client_map_mutex_);
			it = client_map_.find(state.uniqueKey);
			if (it == client_map_.end())
			{
				break;
			}
			
		}
		room_number = it->second.GetRoomNumber();
		room_manager_.ExitRoom(room_number, state.uniqueKey);

		Room room;
		bool get_room = room_manager_.GetRoom(room_number, room);
		if (get_room == false)
			return;

		for (const int& user_id : room.GetUserList())
		{
			auto find_result = client_map_.find(user_id);
			if (find_result != client_map_.end())
			{
				OnRoomExitOtherPacket exit_other_packet;
				exit_other_packet.client_data.client_number = it->second.GetClientNumber();
				exit_other_packet.client_data.client_name = it->second.GetClientName();
				server_socket_.SendPacketToClient(find_result->first, exit_other_packet);
			}
		}
		break;
	} 

	case ObserveRoomListPacket::StaticPacketID:
	{
		ObserveRoomListPacket* observe_room_list_packet = static_cast<ObserveRoomListPacket*>(packet.get());
		std::lock_guard<std::mutex> lock(client_map_mutex_);
		auto it = client_map_.find(state.uniqueKey);
		if (it != client_map_.end())
		{
			it->second.SetObserveRoomList(observe_room_list_packet->is_observe);
		}
		break;
	}
	
	default:
		break;
	}
}

void ServerManager::OnRoomListChanged(Room changed_room, RoomListUpdateType type)
{
	RoomInfo room_info;
	room_info.room_number = changed_room.GetRoomNumber();
	room_info.current_user_count = changed_room.GetCurrentUserCount();
	room_info.max_user_count = changed_room.GetMaxUserCount();
	room_info.room_title = changed_room.GetRoomTitle();

	OnUpdateRoomListPacket update_room_list_packet;
	update_room_list_packet.room_info = std::move(room_info);
	update_room_list_packet.update_type = type;

	{
		std::lock_guard<std::mutex> lock(client_map_mutex_);
		for (const auto& client : client_map_)
		{
			if (client.second.IsObserveRoomList())
			{
				server_socket_.SendPacketToClient(client.first, update_room_list_packet);
			}
		}
	}
}
