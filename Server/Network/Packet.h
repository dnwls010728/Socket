#pragma once
class Packet
{
protected:
	virtual BYTE* Serialize() { return nullptr; };
	virtual Packet* Deserialize() { return nullptr; };
};


class EnterPacket : public Packet
{
public:
	EnterPacket();
protected:
	virtual BYTE* Serialize() override;
	virtual Packet* Deserialize() override;

private:
	string _name;
	string _id;
};