#pragma once
#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
class EXPORT_API DeSerializer
{
public:
    void Deserialize(BYTE* pos, int& currentByte, uint8_t& value);
    void Deserialize(BYTE* pos, int& currentByte, uint16_t& value);
    void Deserialize(BYTE* pos, int& currentByte, uint32_t& value);
    void Deserialize(BYTE* pos, int& currentByte, int32_t& value);
    void Deserialize(BYTE* pos, int& currentByte, uint64_t& value);
    void Deserialize(BYTE* pos, int& currentByte, int64_t& value);
    void Deserialize(BYTE* pos, int& currentByte, std::string& value);
    void Deserialize(BYTE* pos, int& currentByte, uint16_t* value);
    void Deserialize(BYTE* pos, int& currentByte, uint32_t* value);
    void Deserialize(BYTE* pos, int& currentByte, int64_t* value);
    void Deserialize(BYTE* pos, int& currentByte, double& value);
    void Deserialize(BYTE* pos, int& currentByte, double* value);
    void Deserialize(BYTE* pos, int& currentByte, float& value);
    void Deserialize(BYTE* pos, int& currentByte, float* value);
    void Deserialize(BYTE* pos, int& currentByte, std::string* value);
    
};

extern DeSerializer* GDeSerializer;