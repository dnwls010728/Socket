#pragma once
class Packet
{
protected:
	virtual BYTE* Serialize(BYTE* buffer) { return nullptr; };
	virtual void Deserialize(BYTE* buffer, int32_t len) {  };
	virtual uint16_t GetSize() { return 0; };
};


class C_EnterPacket : public Packet
{
public:
	C_EnterPacket();
	
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;

private:
	string _name;
	string _id;
};

class S_EnterPacket : public Packet
{
public:
	S_EnterPacket();
	virtual BYTE* Serialize(BYTE* buffer) override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
	virtual uint16_t GetSize() override;
private:
	uint8_t _success;
};