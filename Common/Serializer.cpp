#include "pch.h"
#include "Serializer.h"



void Serializer::Serialize(BYTE* pos, uint8_t value, int& currentByte)
{
    pos[currentByte] = 1;
    pos[currentByte+1] = static_cast<BYTE>(value);
    currentByte += sizeof(value);
}

void Serializer::Serialize(BYTE* pos, uint16_t value, int& currentByte)
{
    pos[currentByte] = 2;
    pos[currentByte+1] = static_cast<BYTE>((value >> 8) & 0xFF);
    pos[currentByte+2] = static_cast<BYTE>((value >> 0) & 0xFF);
    currentByte += sizeof(value);
}

void Serializer::Serialize(BYTE* pos, uint32_t value, int& currentByte)
{
    pos[currentByte] = 3;
    pos[currentByte+1] = static_cast<BYTE>((value >> 0) & 0xFF);
    pos[currentByte+2] = static_cast<BYTE>((value >> 8) & 0xFF);
    pos[currentByte+3] = static_cast<BYTE>((value >> 16) & 0xFF);
    pos[currentByte+4] = static_cast<BYTE>((value >> 24) & 0xFF);
    currentByte += sizeof(value)+1;
}

void Serializer::Serialize(BYTE* pos, int32_t value, int& currentByte)
{
    pos[currentByte] = 4;
    pos[currentByte + 1] = static_cast<BYTE>((value >> 0) & 0xFF);
    pos[currentByte + 2] = static_cast<BYTE>((value >> 8) & 0xFF);
    pos[currentByte + 3] = static_cast<BYTE>((value >> 16) & 0xFF);
    pos[currentByte + 4] = static_cast<BYTE>((value >> 24) & 0xFF);
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, uint64_t value, int& currentByte)
{
    pos[currentByte] = 5;
    pos[currentByte + 1] = static_cast<BYTE>((value >> 0) & 0xFF);
    pos[currentByte + 2] = static_cast<BYTE>((value >> 8) & 0xFF);
    pos[currentByte + 3] = static_cast<BYTE>((value >> 16) & 0xFF);
    pos[currentByte + 4] = static_cast<BYTE>((value >> 24) & 0xFF);
    pos[currentByte + 5] = static_cast<BYTE>((value >> 32) & 0xFF);
    pos[currentByte + 6] = static_cast<BYTE>((value >> 40) & 0xFF);
    pos[currentByte + 7] = static_cast<BYTE>((value >> 48) & 0xFF);
    pos[currentByte + 8] = static_cast<BYTE>((value >> 56) & 0xFF);
    currentByte += sizeof(value) + 1;
}

void Serializer::Serialize(BYTE* pos, int64_t value, int& currentByte)
{
    pos[currentByte] = 6;
    pos[currentByte + 1] = static_cast<BYTE>((value >> 0) & 0xFF);
    pos[currentByte + 2] = static_cast<BYTE>((value >> 8) & 0xFF);
    pos[currentByte + 3] = static_cast<BYTE>((value >> 16) & 0xFF);
    pos[currentByte + 4] = static_cast<BYTE>((value >> 24) & 0xFF);
    pos[currentByte + 5] = static_cast<BYTE>((value >> 32) & 0xFF);
    pos[currentByte + 6] = static_cast<BYTE>((value >> 40) & 0xFF);
    pos[currentByte + 7] = static_cast<BYTE>((value >> 48) & 0xFF);
    pos[currentByte + 8] = static_cast<BYTE>((value >> 56) & 0xFF);
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
    currentByte += strlen(value.c_str());
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
        pos[currentByte] = static_cast<BYTE>((arr[idx] >> 0) & 0xFF);
        pos[currentByte + 1] = static_cast<BYTE>((arr[idx] >> 8) & 0xFF);
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
        pos[currentByte] = static_cast<BYTE>((arr[idx] >> 0) & 0xFF);
        pos[currentByte + 1] = static_cast<BYTE>((arr[idx] >> 8) & 0xFF);
        pos[currentByte + 2] = static_cast<BYTE>((arr[idx] >> 16) & 0xFF);
        pos[currentByte + 3] = static_cast<BYTE>((arr[idx] >> 24) & 0xFF);
        currentByte += sizeof(arr[idx]);
    }
    
}

void Serializer::Serialize(BYTE* pos, int64_t* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 10;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        pos[currentByte] = static_cast<BYTE>((arr[idx] >> 0) & 0xFF);
        pos[currentByte + 1] = static_cast<BYTE>((arr[idx] >> 8) & 0xFF);
        pos[currentByte + 2] = static_cast<BYTE>((arr[idx] >> 16) & 0xFF);
        pos[currentByte + 3] = static_cast<BYTE>((arr[idx] >> 24) & 0xFF);
        pos[currentByte + 4] = static_cast<BYTE>((arr[idx] >> 32) & 0xFF);
        pos[currentByte + 5] = static_cast<BYTE>((arr[idx] >> 40) & 0xFF);
        pos[currentByte + 6] = static_cast<BYTE>((arr[idx] >> 48) & 0xFF);
        pos[currentByte + 7] = static_cast<BYTE>((arr[idx] >> 56) & 0xFF);
        currentByte += sizeof(arr[idx]);
    }
}

void Serializer::Serialize(BYTE* pos, double value, int& currentByte)
{
    uint64_t casting = static_cast<uint64_t>(value);
    pos[currentByte] = 11;
    pos[currentByte + 1] = static_cast<BYTE>((casting >> 0) & 0xFF);
    pos[currentByte + 2] = static_cast<BYTE>((casting >> 8) & 0xFF);
    pos[currentByte + 3] = static_cast<BYTE>((casting >> 16) & 0xFF);
    pos[currentByte + 4] = static_cast<BYTE>((casting>> 24) & 0xFF);
    pos[currentByte + 5] = static_cast<BYTE>((casting >> 32) & 0xFF);
    pos[currentByte + 6] = static_cast<BYTE>((casting >> 40) & 0xFF);
    pos[currentByte + 7] = static_cast<BYTE>((casting >> 48) & 0xFF);
    pos[currentByte + 8] = static_cast<BYTE>((casting >> 56) & 0xFF);
    currentByte += sizeof(value) + 1;
    
}

void Serializer::Serialize(BYTE* pos, double* arr, uint8_t len, int& currentByte)
{
    pos[currentByte] = 12;
    currentByte++;
    pos[currentByte] = len;
    currentByte++;
    for (uint8_t idx = 0; idx < len; idx++) {
        uint64_t casting = arr[idx];
        pos[currentByte] = static_cast<BYTE>((casting>> 0) & 0xFF);
        pos[currentByte + 1] = static_cast<BYTE>((casting >> 8) & 0xFF);
        pos[currentByte + 2] = static_cast<BYTE>((casting >> 16) & 0xFF);
        pos[currentByte + 3] = static_cast<BYTE>((casting >> 24) & 0xFF);
        pos[currentByte + 4] = static_cast<BYTE>((casting >> 32) & 0xFF);
        pos[currentByte + 5] = static_cast<BYTE>((casting >> 40) & 0xFF);
        pos[currentByte + 6] = static_cast<BYTE>((casting >> 48) & 0xFF);
        pos[currentByte + 7] = static_cast<BYTE>((casting >> 56) & 0xFF);
        currentByte += sizeof(casting);
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
    // ±¸ºÐÀÚ 1¹ÙÀÌÆ®, ¹®ÀÚ¿­ ±æÀÌ 1¹ÙÀÌÆ®, ³Î¹®ÀÚ 1¹ÙÀÌÆ®
    return strlen(value.c_str()) + 3;
}

int Serializer::GetPacketSize(uint16_t* arr, uint8_t len)
{
    //±¸ºÐÀÚ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ * ÀÚ·áÇü Å©±â
    return sizeof(arr[0]) * len + 2;
}

int Serializer::GetPacketSize(uint32_t* arr, uint8_t len)
{
    //±¸ºÐÀÚ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ * ÀÚ·áÇü Å©±â
    return sizeof(arr[0]) * len + 2;
}

int Serializer::GetPacketSize(uint64_t* arr, uint8_t len)
{
    //±¸ºÐÀÚ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ * ÀÚ·áÇü Å©±â
    return sizeof(arr[0]) * len + 2;
}

int Serializer::GetPacketSize(double value)
{
    return sizeof(value) + 1;
}

int Serializer::GetPacketSize(double* arr, uint8_t len)
{
    //±¸ºÐÀÚ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ 1¹ÙÀÌÆ®, ¹è¿­ ±æÀÌ * ÀÚ·áÇü Å©±â
    return sizeof(arr[0]) *len + 2;
}
