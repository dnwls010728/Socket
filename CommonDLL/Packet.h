#pragma once

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

class EXPORT_API Packet
{
public:
	Packet()
	{
        
	}
	virtual ~Packet()
	{
        
	}
protected:
    
    
	virtual BYTE* Serialize(BYTE* buffer) { return nullptr; }
	virtual void Deserialize(BYTE* buffer, int32_t len) {  }
	virtual uint16_t GetSize() { return 0; }
	
};


class EXPORT_API C_Enter : public Packet
{
public:
	C_Enter() =default;

	~C_Enter() override = default;    

	
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;
	


	std::string _name;
	std::string _id;
};

class EXPORT_API S_Enter : public Packet
{
public:
	S_Enter() = default;
	~S_Enter() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	
	uint8_t _success;
	uint32_t _userId;
	std::string _name;
};

class EXPORT_API C_Moving : public Packet
{
public:
	C_Moving() = default;
	~C_Moving() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	
	float _locationX;
	float _locationY;
	

	
	
};

class EXPORT_API S_Moving : public Packet
{
public:
	S_Moving() = default;
	~S_Moving() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	

	uint8_t _success;
	uint32_t _userId;
	float _locationX;
	float _locationY;
};

class EXPORT_API S_BroadcastingEnter : public Packet
{
public:
	S_BroadcastingEnter() = default;
	~S_BroadcastingEnter() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;


	
	uint8_t _success;
	uint32_t _userId;
	std::string _name;
};

class EXPORT_API S_EnterOtherUser : public Packet
{
public:
	S_EnterOtherUser() = default;
	~S_EnterOtherUser() override =default;
	
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	
	
	uint32_t* userIdentifyidArr_;
	std::string* nameArr_;
	float* locationXArr_;
	float* locationYArr_;
	uint32_t currentUserCnt_;
};

class EXPORT_API C_EnterOtherUser : public Packet
{
public:
	C_EnterOtherUser() = default;
	~C_EnterOtherUser() override = default;

	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _userId;
};

class EXPORT_API S_LeaveOtherUser : public Packet
{
public:
	S_LeaveOtherUser() = default;
	~S_LeaveOtherUser() override = default;

	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _userId;
};

class EXPORT_API C_EnterRoom : public Packet
{
public:
	C_EnterRoom() = default;
	~C_EnterRoom() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _roomNum;
	
};

class EXPORT_API S_EnterRoom : public Packet
{
public:
	S_EnterRoom() = default;
	~S_EnterRoom() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _currentRoomNum;
};

class EXPORT_API C_EnterChannel : public Packet
{
	public:
	C_EnterChannel() = default;
	~C_EnterChannel() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _channelNum;
};

class EXPORT_API S_EnterChannel : public Packet
{
	public:
	S_EnterChannel() = default;
	~S_EnterChannel() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;
	
	uint32_t _currentChannelNum;
};

class EXPORT_API S_LeaveChannel : public Packet
{
public:
	S_LeaveChannel() = default;
	~S_LeaveChannel() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint32_t _userId;
};