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

class EXPORT_API SendBufferChunk;

extern thread_local std::shared_ptr<class SendBufferChunk> LSendBufferChunk;

/*----------------
	SendBuffer
-----------------*/

class EXPORT_API SendBuffer
{
public:
    SendBuffer(std::shared_ptr<SendBufferChunk> owner, BYTE* buffer, uint32_t allocSize);
    ~SendBuffer();

    BYTE* Buffer() { return _buffer; }
    uint32_t AllocSize() { return _allocSize; }
    uint32_t WriteSize() { return _writeSize; }
    void Close(uint32_t writeSize);

private:
    BYTE* _buffer;
    uint32_t _allocSize = 0;
    uint32_t _writeSize = 0;
    std::shared_ptr<SendBufferChunk> _owner;
};

/*--------------------
	SendBufferChunk
--------------------*/

class EXPORT_API SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
{
    enum
    {
        SEND_BUFFER_CHUNK_SIZE = 6000
    };

public:
    SendBufferChunk();
    ~SendBufferChunk();

    void Reset();
    std::shared_ptr<SendBuffer> Open(uint32_t allocSize);
    void Close(uint32_t writeSize);

    bool IsOpen() { return _open; }
    BYTE* Buffer() { return &_buffer[_usedSize]; }
    uint32_t FreeSize() { return static_cast<uint32_t>(_buffer.size()) - _usedSize; }

private:
    std::array<BYTE, SEND_BUFFER_CHUNK_SIZE> _buffer = {};
    bool _open = false;
    uint32_t _usedSize = 0;
};

/*---------------------
	SendBufferManager
----------------------*/

class EXPORT_API SendBufferManager
{
public:
    std::shared_ptr<SendBuffer> Open(uint32_t size);

private:
    std::shared_ptr<SendBufferChunk> Pop();
    void Push(std::shared_ptr<SendBufferChunk> buffer);

    static void PushGlobal(SendBufferChunk* buffer);

private:
    std::mutex _mutex;
    std::vector<std::shared_ptr<SendBufferChunk>> _sendBufferChunks;
};

extern EXPORT_API SendBufferManager* GSendBufferManager;
