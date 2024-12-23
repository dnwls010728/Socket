#pragma once
#include <memory>
#include <mutex>
#include <vector>

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

/*--------------
	RecvBuffer
----------------*/

class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	EXPORT_API RecvBuffer(int32_t bufferSize);
	EXPORT_API ~RecvBuffer();

	EXPORT_API void			Clean();
	EXPORT_API bool			OnRead(int32_t numOfBytes);
	EXPORT_API bool			OnWrite(int32_t numOfBytes);

	EXPORT_API BYTE* ReadPos() { return &_buffer[_readPos]; }
	EXPORT_API BYTE* WritePos() { return &_buffer[_writePos]; }
	EXPORT_API int32_t			DataSize() { return _writePos - _readPos; }
	EXPORT_API int32_t			FreeSize() { return _capacity - _writePos; }

private:
	int32_t			_capacity = 0;
	int32_t			_bufferSize = 0;
	int32_t			_readPos = 0;
	int32_t			_writePos = 0;
	std::vector<BYTE>	_buffer;
};