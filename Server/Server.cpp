#include "ServerManager.h"
#define _CRTDBG_MAP_ALLOC

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	ServerManager::Get()->Execute();

	return 0;
} 