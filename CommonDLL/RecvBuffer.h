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

class EXPORT_API RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32_t bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32_t numOfBytes);
	bool			OnWrite(int32_t numOfBytes);

	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	int32_t			DataSize() { return _writePos - _readPos; }
	int32_t			FreeSize() { return _capacity - _writePos; }

private:
	int32_t			_capacity = 0;
	int32_t			_bufferSize = 0;
	int32_t			_readPos = 0;
	int32_t			_writePos = 0;
	std::vector<BYTE>	_buffer;
};