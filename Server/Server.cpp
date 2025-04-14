// Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <iostream>
#include "ServerManager.h"
#include "Serializer.h"
int main()
{
	ServerManager server_manager;
	server_manager.Execute();
	return 0;
} 