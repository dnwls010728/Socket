#pragma once
class Packet
{
public:
	Packet()
	{
        
	};
	virtual ~Packet()
	{
        
	};
protected:
    
    
	virtual BYTE* Serialize(BYTE* buffer) { return nullptr; };
	virtual void Deserialize(BYTE* buffer, int32_t len) {  };
	virtual uint16_t GetSize() { return 0; };
};


class C_EnterPacket : public Packet
{
public:
	C_EnterPacket();

	~C_EnterPacket() override = default;    

	void SetName(const std::string& name){_name=name;};
	void SetId(const std::string& id){_id=id;};
    std::string GetName() const{return _name;};
	std::string GetId() const{return _id;};
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;


	std::string _name;
	std::string _id;
};

class S_EnterPacket : public Packet
{
public:

	void SetSuccess(uint8_t success){_success=success;};
    uint8_t GetSuccess()const{return _success;};
	S_EnterPacket();
	~S_EnterPacket() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	uint8_t _success;
	uint32_t _userId;
	std::string _name;
};

class C_MovingPacket : public Packet
{
public:
	C_MovingPacket();
	~C_MovingPacket() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

	float _locationX;
	float _locationY;
	

	
	
};

class S_MovingPacket : public Packet
{
public:
	S_MovingPacket();
	~S_MovingPacket() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;


	uint8_t _success;
	uint32_t _userId;
	float _locationX;
	float _locationY;
};

class S_BroadcastingEnterPacket : public Packet
{
public:
	S_BroadcastingEnterPacket();
	~S_BroadcastingEnterPacket() override = default;
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;


	uint8_t _success;
	uint32_t _userId;
	std::string _name;
};