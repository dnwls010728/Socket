#pragma once
#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
class EXPORT_API DeSerializer
{
public:
    static void Deserialize(BYTE* pos, int& currentByte, uint8_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, uint16_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, uint32_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, int32_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, uint64_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, int64_t& value);
    static void Deserialize(BYTE* pos, int& currentByte, std::string& value);
    static void Deserialize(BYTE* pos, int& currentByte, uint16_t* value);
    static void Deserialize(BYTE* pos, int& currentByte, uint32_t* value);
    static void Deserialize(BYTE* pos, int& currentByte, int64_t* value);
    static void Deserialize(BYTE* pos, int& currentByte, double& value);
    static void Deserialize(BYTE* pos, int& currentByte, double* value);
    static void Deserialize(BYTE* pos, int& currentByte, float& value);
    static void Deserialize(BYTE* pos, int& currentByte, float* value);
    static void Deserialize(BYTE* pos, int& currentByte, std::string* value);

    static void HeapInitialize(uint16_t** arr, int len);
    static void HeapInitialize(uint32_t** arr, int len);
    static void HeapInitialize(int64_t** arr, int len);
    static void HeapInitialize(double** arr, int len);
    static void HeapInitialize(float** arr, int len);
    static void HeapInitialize(std::string** arr, int len);
    
};