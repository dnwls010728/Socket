#include "pch.h"
#include "CoreTLS.h"


thread_local uint32_t LThreadId = 0;
thread_local uint64_t LEndTickCount = 0;
thread_local JobQueue* LCurrentJobQueue = nullptr;
thread_local std::shared_ptr<class SendBufferChunk> LSendBufferChunk;