#pragma once

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

class EXPORT_API Packet
{
public:
	Packet() = default;
	Packet(const Packet& rhs)
	{
		*this = rhs;
	}
	Packet& operator=(const Packet& rhs) = default;
	Packet(Packet&& rhs) noexcept
	{
		*this = std::move(rhs);
	}
	Packet& operator=(Packet&& rhs) = default;
	virtual ~Packet()= default;
	
protected:
	virtual BYTE* Serialize(BYTE* buffer) { return nullptr; }
	virtual void Deserialize(BYTE* buffer, int32_t len) {  }
	virtual uint16_t GetSize() { return 0; }
	
	
};