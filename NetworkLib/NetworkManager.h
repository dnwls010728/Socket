#pragma once
#include <winsock2.h>
#include "NetDef.h"

namespace Net {

	// 윈도우 WSA 초기화. 소켓 사용을 위해 호출 필요. 프로그램 시작시 한번만 호출.
	bool WSAInit();

	// 윈도우 WSA 해제. WSAInit가 호출 된 만큼 호출해야 함.
	void WSAUninit();

} // namespace Net