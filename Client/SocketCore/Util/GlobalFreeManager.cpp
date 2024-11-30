#include "pch.h"
#include "GlobalFreeManager.h"

#include "../SocketSession.h"
SocketSession* GSocketSession = nullptr;

void GlobalFreeManager::FreeMemory()
{
    delete GSocketSession;
}

void GlobalFreeManager::InitMemory()
{
    GSocketSession = new SocketSession();
}
