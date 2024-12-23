#pragma once

extern class SocketSession* GSocketSession;


class GlobalFreeManager
{

public:
    static void FreeMemory();
    static void InitMemory();
};

