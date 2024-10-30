#pragma once
class Serializer
{
public:
    static void Serialize(BYTE* pos, uint8_t value, int& currentByte);
    static void Serialize(BYTE* pos,uint16_t value, int& currentByte);
    static void Serialize(BYTE* pos, uint32_t value, int& currentByte);
    static void Serialize(BYTE* pos, int32_t value, int& currentByte);
    static void Serialize(BYTE* pos, uint64_t value, int& currentByte);
    static void Serialize(BYTE* pos, int64_t value, int& currentByte);
    static void Serialize(BYTE* pos, std::string value, int& currentByte);
    static void Serialize(BYTE* pos, uint16_t* arr, uint8_t len, int& currentByte);
    static void Serialize(BYTE* pos, uint32_t* arr, uint8_t len, int& currentByte);
    static void Serialize(BYTE* pos, int64_t* arr, uint8_t len, int& currentByte);
    static void Serialize(BYTE* pos, double value, int& currentByte);
    static void Serialize(BYTE* pos, double* arr, uint8_t len, int& currentByte);
	static void Serialize(BYTE* pos, float value, int& currentByte);
	static void Serialize(BYTE* pos, float* arr, uint8_t len, int& currentByte);

    static int GetPacketSize(uint8_t value);
    static int GetPacketSize(uint16_t value);
    static int GetPacketSize(uint32_t value);
    static int GetPacketSize(int32_t value);
    static int GetPacketSize(uint64_t value);
    static int GetPacketSize(int64_t value);
    static int GetPacketSize(std::string value);
    static int GetPacketSize(uint16_t* arr,uint8_t len);
    static int GetPacketSize(uint32_t* arr, uint8_t len );
    static int GetPacketSize(uint64_t* arr, uint8_t len );
    static int GetPacketSize(double value);
    static int GetPacketSize(double* arr, uint8_t len);
	static int GetPacketSize(float value);
	static int GetPacketSize(float* arr, uint8_t len);

	
};

