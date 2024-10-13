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

#include "..\Thread\Lock.h"
#include "..\Network\Session.h"
#include "..\Thread\ThreadManager.h"
#include "..\Network\Service.h"
#include "..\Job\JobQueue.h"
#include "..\Util\SendBuffer.h"
#include "..\Util\RecvBuffer.h"
#include "..\Network\Packet.h"