#pragma once

#include <array>
#include <memory>
#include <mutex>
#include <vector>

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

class SendBufferChunk;

extern thread_local std::shared_ptr<class SendBufferChunk> LSendBufferChunk;

/*----------------
	SendBuffer
-----------------*/

class SendBuffer
{
public:
    EXPORT_API SendBuffer(std::shared_ptr<SendBufferChunk> owner, BYTE* buffer, uint32_t allocSize);
    EXPORT_API ~SendBuffer();

    EXPORT_API BYTE* Buffer() { return _buffer; }
    EXPORT_API uint32_t AllocSize() { return _allocSize; }
    EXPORT_API uint32_t WriteSize() { return _writeSize; }
    EXPORT_API void Close(uint32_t writeSize);

private:
    BYTE* _buffer;
    uint32_t _allocSize = 0;
    uint32_t _writeSize = 0;
    std::shared_ptr<SendBufferChunk> _owner;
};

/*--------------------
	SendBufferChunk
--------------------*/

class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
{
    enum
    {
        SEND_BUFFER_CHUNK_SIZE = 6000
    };

public:
    EXPORT_API SendBufferChunk();
    EXPORT_API ~SendBufferChunk();

    EXPORT_API void Reset();
    EXPORT_API std::shared_ptr<SendBuffer> Open(uint32_t allocSize);
    EXPORT_API void Close(uint32_t writeSize);

    EXPORT_API bool IsOpen() { return _open; }
    EXPORT_API BYTE* Buffer() { return &_buffer[_usedSize]; }
    EXPORT_API uint32_t FreeSize() { return static_cast<uint32_t>(_buffer.size()) - _usedSize; }

private:
    std::array<BYTE, SEND_BUFFER_CHUNK_SIZE> _buffer = {};
    bool _open = false;
    uint32_t _usedSize = 0;
};

/*---------------------
	SendBufferManager
----------------------*/

class SendBufferManager
{
public:
    EXPORT_API std::shared_ptr<SendBuffer> Open(uint32_t size);

private:
    EXPORT_API std::shared_ptr<SendBufferChunk> Pop();
    EXPORT_API void Push(std::shared_ptr<SendBufferChunk> buffer);

    EXPORT_API static void PushGlobal(SendBufferChunk* buffer);

private:
    std::mutex _mutex;
    std::vector<std::shared_ptr<SendBufferChunk>> _sendBufferChunks;
};

extern EXPORT_API SendBufferManager* GSendBufferManager;
