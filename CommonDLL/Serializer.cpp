#include "pch.h"
#include "Serializer.h"
Serializer* GSerializer = new Serializer();

void Serializer::Serialize(BYTE* pos, uint8_t value, int& currentByte)
{
    pos[currentByte] = 1;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value)+1;
}

void Serializer::Serialize(BYTE* pos, uint16_t value, int& currentByte)
{
    pos[currentByte] = 2;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value)+1;
}

void Serializer::Serialize(BYTE* pos, uint32_t value, int& currentByte)
{
    pos[currentByte] = 3;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value)+1;
}

void Serializer::Serialize(BYTE* pos, int32_t value, int& currentByte)
{
    pos[currentByte] = 4;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, uint64_t value, int& currentByte)
{
    pos[currentByte] = 5;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, int64_t value, int& currentByte)
{
    pos[currentByte] = 6;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, std::string value, int& currentByte)
{
    pos[currentByte] = 7;
    currentByte++;
    uint8_t strLen = static_cast<uint8_t>(strlen(value.c_str()));
    pos[currentByte] = strLen;
    currentByte++;
    strcpy_s(reinterpret_cast<char*>(pos + currentByte), strlen(value.c_str()) + 1, value.c_str());
    currentByte += static_cast<int>(strlen(value.c_str()));
    pos[currentByte] = '\0';
    currentByte++;

    
}



void Serializer::Serialize(BYTE* pos, uint16_t* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 8;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++)
    {
        std::memcpy(&pos[currentByte], &arr[idx], sizeof(arr[idx]));
        currentByte += sizeof(arr[idx]);
    }
}

void Serializer::Serialize(BYTE* pos, uint32_t* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 9;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        std::memcpy(&pos[currentByte], &arr[idx], sizeof(arr[idx]));
        currentByte += sizeof(uint32_t);
    }
    
}

void Serializer::Serialize(BYTE* pos, int64_t* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 10;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        std::memcpy(&pos[currentByte], &arr[idx], sizeof(arr[idx]));
        currentByte += sizeof(arr[idx]);
    }
}

void Serializer::Serialize(BYTE* pos, double value, int& currentByte)
{
    pos[currentByte] = 11;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value) + 1;
    
}

void Serializer::Serialize(BYTE* pos, double* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 12;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        std::memcpy(&pos[currentByte], &arr[idx], sizeof(arr[idx]));
        currentByte += sizeof(arr[idx]);
    }
}

void Serializer::Serialize(BYTE* pos, float value, int& currentByte)
{
    pos[currentByte] = 13;
    std::memcpy(&pos[currentByte+1], &value, sizeof(value));
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, float* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 14;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        std::memcpy(&pos[currentByte], &arr[idx], sizeof(arr[idx]));
        currentByte += sizeof(arr[idx]);
    }
}

void Serializer::Serialize(BYTE* pos, std::string* arr,uint8_t len, int& currentByte)
{
    pos[currentByte] = 15;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for(uint8_t idx = 0;idx<len;idx++)
    {
        pos[currentByte] = static_cast<BYTE>(strlen(arr[idx].c_str()));
        currentByte++;
        strcpy_s(reinterpret_cast<char*>(pos + currentByte), strlen(arr[idx].c_str()) + 1, arr[idx].c_str());
        currentByte += static_cast<BYTE>(strlen(arr[idx].c_str()));
        pos[currentByte] = '\0';
        currentByte++;
    }
}

int Serializer::GetPacketSize(uint8_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(uint16_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(uint32_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(int32_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(uint64_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(int64_t value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(std::string value)
{
    // 식별자 1 + 문자열 길이 + 1 null문자 + 1
    return static_cast<int>(strlen(value.c_str()) + 3);
}

int Serializer::GetPacketSize(std::string* arr,uint8_t len)
{
    //식별자 1 + 배열 길이 + 1  (문자열 길이 +1 null문자 +1) * 배열의 길이 만큼
    int temp = 0;
    temp += 1;
    temp += 1;
    for(uint8_t idx = 0;idx<len;idx++)
    {
        temp += static_cast<int>(strlen(arr[idx].c_str()) + 1);
    }
    return temp;
}

int Serializer::GetPacketSize(uint16_t* arr, uint8_t len)
{
    
    return sizeof(arr[0]) * len + 2;
}

int Serializer::GetPacketSize(uint32_t* arr, uint8_t len)
{
    
    return static_cast<int>(sizeof(arr[0]) * len + 2);
}

int Serializer::GetPacketSize(uint64_t* arr, uint8_t len)
{
    
    return sizeof(arr[0]) * len + 2;
}

int Serializer::GetPacketSize(double value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(double* arr, uint8_t len)
{
    
    return static_cast<int>(sizeof(arr[0]) *len + 2);
}

int Serializer::GetPacketSize(float value)
{
    return sizeof(value)+1;
}

int Serializer::GetPacketSize(float* arr, uint8_t len)
{
    return static_cast<int>(sizeof(float) * len + 2);
}
