#include "pch.h"
#include "CoreGlobal.h"
#include "..\Thread\ThreadManager.h"
#include "..\Job\GlobalQueue.h"
#include "..\Network\SocketUtils.h"
#include "..\Job\JobTimer.h"
#include "SendBuffer.h"

ThreadManager* GThreadManager = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
JobTimer* GJobTimer = nullptr;
SendBufferManager* GSendBufferManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GGlobalQueue = new GlobalQueue();
		GJobTimer = new JobTimer();
		GSendBufferManager = new SendBufferManager();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GGlobalQueue;
		delete GJobTimer;
		delete GSendBufferManager;
		SocketUtils::Clear();
	}
}GCoreGlobal;