#include "RoomManager.h"
#include <algorithm>

// ------------------ Room ------------------
Room::Room() : room_number_(0), max_user_count_(3) {}
Room::~Room() {}

bool Room::EnterUser(int user_id)
{
	if (user_list_.size() >= max_user_count_)
		return false;

	user_list_.insert(user_id);
	return true;
}

bool Room::ExitUser(int user_id)
{
	return user_list_.erase(user_id) > 0;
}

bool Room::IsUserInRoom(int user_id) const
{
	return user_list_.find(user_id) != user_list_.end();
}

std::unordered_set<int> Room::GetUserList()
{
	return user_list_;
}

// ------------------ RoomManager ------------------
RoomManager::RoomManager() : last_room_number_(0), room_list_update_callback_(nullptr){}
RoomManager::~RoomManager() {}

int RoomManager::CreateRoom(std::wstring title, int max_user_count)
{
	std::lock_guard lock(mtx_);

	int room_number = ++last_room_number_;
	Room new_room;
	new_room.SetRoomNumber(room_number);
	new_room.SetRoomTitle(title);
	new_room.SetMaxUserCount(max_user_count);

	auto [it, result] = rooms_.emplace(room_number, new_room);
	if (!result)
		return -1;

	if (room_list_update_callback_)
	{
		room_list_update_callback_(it->second, kRoomAdd);
	}

	return room_number;
}

void RoomManager::RemoveRoom(int room_number)
{
	std::lock_guard lock(mtx_);

	size_t erased = rooms_.erase(room_number);
	if (room_list_update_callback_)
	{
		Room room;
		room.SetRoomNumber(room_number);	
		room_list_update_callback_(room, kRoomRemove);
	}
}

bool RoomManager::EnterRoom(int room_number, int user_id)
{
	std::lock_guard lock(mtx_);

	auto it = rooms_.find(room_number);
	if (it == rooms_.end())
		return false;

	if (it->second.GetCurrentUserCount() >= it->second.GetMaxUserCount())
		return false;

	bool result = it->second.EnterUser(user_id);
	if (result)
	{
		if (room_list_update_callback_)
		{
			room_list_update_callback_(it->second, kRoomModify);
		}
	}
	return result;
}

bool RoomManager::ExitRoom(int room_number, int user_id)
{
	std::lock_guard lock(mtx_);

	auto it = rooms_.find(room_number);
	if (it == rooms_.end())
		return false;

	Room& room = it->second;
	bool result = room.ExitUser(user_id);

	if (result && room.GetCurrentUserCount() == 0)
	{
		RemoveRoom(room.GetRoomNumber());
		if (room_list_update_callback_)
		{
			room_list_update_callback_(room, kRoomRemove);
		}
	}
	else if (result)
	{
		if (room_list_update_callback_)
		{
			room_list_update_callback_(room, kRoomModify);
		}
	}
	return result;
}

bool RoomManager::GetRoom(int room_number, Room& room)
{
	std::lock_guard lock(mtx_);

	auto it = rooms_.find(room_number);
	if (it == rooms_.end())
		return false;

	room = it->second;
	return true;
}

RoomList RoomManager::GetRoomList()
{
	std::lock_guard lock(mtx_);

	RoomList list;
	for (const auto& [room_number, room] : rooms_)
	{
		RoomInfo room_info;
		room_info.room_number = room.GetRoomNumber();
		room_info.current_user_count = room.GetCurrentUserCount();
		room_info.max_user_count = room.GetMaxUserCount();
		room_info.room_title = room.GetRoomTitle();
		list.room_list.push_back(std::move(room_info));
	}
	
	return list;
}
