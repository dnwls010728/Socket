#pragma once

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <mutex>
#include <atomic>
using BYTE = unsigned char;
#include "..\Util\CoreMacro.h"
#include "..\Util\CoreTLS.h"
#include "..\Util\CoreGlobal.h"
#include <array>
#include <vector>
#include <queue>
#include <set>

#include <cstdlib>
#include <ctime>

#include <windows.h>
#include <iostream>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\CommonDLL.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\CommonDLL.lib")
#endif

#include "..\Thread\Lock.h"
#include "..\Network\Session.h"
#include "..\Thread\ThreadManager.h"
#include "..\Network\Service.h"
#include "..\Job\JobQueue.h"
#include "../CommonDLL/SendBuffer.h"
#include "../CommonDLL/RecvBuffer.h"
#include "../CommonDLL/Packet.h"