#pragma	 once
#include <vector>
#include <string>
struct RoomInfo
{
	int room_number;
	int current_user_count;
	int max_user_count;
	std::wstring room_title;
};

struct RoomList
{
	int page_index;
	int room_count;
	std::vector<RoomInfo> room_list;
};