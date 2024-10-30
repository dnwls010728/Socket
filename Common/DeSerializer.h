#pragma once
class DeSerializer
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
};