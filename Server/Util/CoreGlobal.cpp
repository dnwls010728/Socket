#include "pch.h"
#include "CoreGlobal.h"

#include "../Database/MySQLConnectionPool.h"
#include "..\Thread\ThreadManager.h"
#include "..\Job\GlobalQueue.h"
#include "..\Network\SocketUtils.h"
#include "..\Job\JobTimer.h"

ThreadManager* GThreadManager = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
JobTimer* GJobTimer = nullptr;
MySQLConnectionPool* GConnectionPool = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GGlobalQueue = new GlobalQueue();
		GJobTimer = new JobTimer();
		GConnectionPool = new MySQLConnectionPool(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME,DB_PORT,DB_POOL_SIZE);
		//GSendBufferManager = new SendBufferManager();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GGlobalQueue;
		delete GJobTimer;
		delete GConnectionPool;
		//delete GSendBufferManager;
		SocketUtils::Clear();
	}
}GCoreGlobal;