#pragma once

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

class EXPORT_API Serializer
{
public:
    void Serialize(BYTE* pos, uint8_t value, int& currentByte);
    void Serialize(BYTE* pos,uint16_t value, int& currentByte);
    void Serialize(BYTE* pos, uint32_t value, int& currentByte);
    void Serialize(BYTE* pos, int32_t value, int& currentByte);
    void Serialize(BYTE* pos, uint64_t value, int& currentByte);
    void Serialize(BYTE* pos, int64_t value, int& currentByte);
    void Serialize(BYTE* pos, std::string value, int& currentByte);
	void Serialize(BYTE* pos, std::string* arr,uint8_t len, int& currentByte);
    void Serialize(BYTE* pos, uint16_t* arr, uint8_t len, int& currentByte);
    void Serialize(BYTE* pos, uint32_t* arr, uint8_t len, int& currentByte);
    void Serialize(BYTE* pos, int64_t* arr, uint8_t len, int& currentByte);
    void Serialize(BYTE* pos, double value, int& currentByte);
    void Serialize(BYTE* pos, double* arr, uint8_t len, int& currentByte);
	void Serialize(BYTE* pos, float value, int& currentByte);
	void Serialize(BYTE* pos, float* arr, uint8_t len, int& currentByte);

    int GetPacketSize(uint8_t value);
    int GetPacketSize(uint16_t value);
    int GetPacketSize(uint32_t value);
    int GetPacketSize(int32_t value);
    int GetPacketSize(uint64_t value);
    int GetPacketSize(int64_t value);
    int GetPacketSize(std::string value);
	int GetPacketSize(std::string* arr,uint8_t len);
    int GetPacketSize(uint16_t* arr,uint8_t len);
    int GetPacketSize(uint32_t* arr, uint8_t len );
    int GetPacketSize(uint64_t* arr, uint8_t len );
    int GetPacketSize(double value);
    int GetPacketSize(double* arr, uint8_t len);
	int GetPacketSize(float value);
	int GetPacketSize(float* arr, uint8_t len);
};

extern Serializer* GSerializer;
