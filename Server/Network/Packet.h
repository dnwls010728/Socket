#pragma once
class Packet
{
protected:
	virtual BYTE* Serialize() { return nullptr; };
	virtual void Deserialize() {  };
};


class C_EnterPacket : public Packet
{
public:
	C_EnterPacket();
protected:
	
	virtual BYTE* Serialize() override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;

private:
	string _name;
	string _id;
};

class S_EnterPacket : public Packet
{
public:
	S_EnterPacket();
protected:
	virtual BYTE* Serialize() override;
	virtual void Deserialize(BYTE* buffer, int32_t len) override;
private:
	bool _success;
};