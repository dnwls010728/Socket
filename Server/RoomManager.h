#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include <mutex>
#include <functional>
#include "CommonObject.h"

// ------------------ Room ------------------
class Room
{
public:
    Room();
    ~Room();

    // User management
    bool EnterUser(int user_id);
    bool ExitUser(int user_id);
    bool IsUserInRoom(int user_id) const;
    std::unordered_set<int> GetUserList();

    // Option
    void SetRoomNumber(int room_number) { room_number_ = room_number; }
    void SetMaxUserCount(int max_user_count) { max_user_count_ = max_user_count; }
    void SetRoomTitle(std::wstring& room_title) { room_title_ = room_title; }

    int GetRoomNumber() const { return room_number_; }
    int GetMaxUserCount() const { return max_user_count_; }
    int GetCurrentUserCount() const { return static_cast<int>(user_list_.size()); }
    std::wstring GetRoomTitle() const { return room_title_; }

private:
    int room_number_;
    int max_user_count_;
    std::wstring room_title_;
    std::unordered_set<int> user_list_;
};

// ------------------ RoomManager ------------------
class RoomManager
{
public:
    RoomManager();
    ~RoomManager();

    // 방 관리
    int CreateRoom(std::wstring title, int max_user_count);
    void RemoveRoom(int room_number);
    bool EnterRoom(int room_number, int user_id);
    bool ExitRoom(int room_number, int user_id);
    bool GetRoom(int room_number, Room& room);

    // 룸 변경 알림 콜백
	void SetRoomListUpdateCallback(std::function<void(Room, RoomListUpdateType)> callback){room_list_update_callback_ = callback;}

    // 방 목록
    RoomList GetRoomList();

private:
    std::map<int, Room> rooms_;
    int last_room_number_ = 0;
    std::mutex mtx_;
	std::function<void(Room, RoomListUpdateType)> room_list_update_callback_;
};
