#pragma	 once
#include <vector>
#include <string>

// 방 찾기(browse)화면에서 제공되는 방 정보
struct RoomInfo
{
	int room_number;
	int current_user_count;
	int max_user_count;
	std::wstring room_title;
};

// 방 찾기(browse)화면에서 제공되는 방 목록
struct RoomList
{
	std::vector<RoomInfo> room_list;
};

// 클라이언트 정보
struct ClientData
{
	int client_number;
	std::wstring client_name;
};

// 방 입장시 제공되는 방 정보
struct RoomInfoEx
{
	int room_number;
	int current_user_count;
	int max_user_count;
	std::wstring room_title;
	std::vector<ClientData> client_list;
};

enum RoomListUpdateType : uint16_t
{
	kRoomAdd,
	kRoomRemove,
	kRoomModify
};