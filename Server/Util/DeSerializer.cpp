#include "pch.h"
#include "DeSerializer.h"

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint8_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 1)
	{
		value = static_cast<uint8_t>(pos[currentByte]);
		currentByte += sizeof(uint8_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint16_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 2)
	{
		value = static_cast<uint16_t>(pos[currentByte]);
		currentByte += sizeof(uint16_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint32_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 3)
	{
		value = static_cast<uint32_t>(pos[currentByte]);
		currentByte += sizeof(uint32_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, int32_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 4)
	{
		value = static_cast<uint8_t>(pos[currentByte]);
		currentByte += sizeof(uint8_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint64_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 5)
	{
		value = static_cast<uint64_t>(pos[currentByte]);
		currentByte += sizeof(uint64_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, int64_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 6)
	{
		value = static_cast<int64_t>(pos[currentByte]);
		currentByte += sizeof(int64_t);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, string& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 7)
	{
		uint8_t strLen = static_cast<uint8_t>(pos[currentByte]);
		currentByte++;
		value = string(reinterpret_cast<const char*>(pos + currentByte));
		currentByte += value.length();
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint16_t* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 8)
	{
		uint8_t len = pos[currentByte];
		currentByte++;
		for (uint8_t idx = 0; idx < len; idx++)
		{
			value[idx] = static_cast<uint16_t>(pos[currentByte]);
			currentByte += sizeof(uint16_t);
		}
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint32_t* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 9)
	{
		uint8_t len = pos[currentByte];
		currentByte++;
		for (uint8_t idx = 0; idx < len; idx++)
		{
			value[idx] = static_cast<uint32_t>(pos[currentByte]);
			currentByte += sizeof(uint32_t);
		}
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, int64_t* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 10)
	{
		uint8_t len = pos[currentByte];
		currentByte++;
		for (uint8_t idx = 0; idx < len; idx++)
		{
			value[idx] = static_cast<int64_t>(pos[currentByte]);
			currentByte += sizeof(int64_t);
		}
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, double& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 11)
	{
		value = static_cast<double>(pos[currentByte]);
		currentByte += sizeof(double);
	}
	else
	{
		CRASH("Deserialize Error");
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, double* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 12)
	{
		uint8_t len = pos[currentByte];
		currentByte++;
		for (uint8_t idx = 0; idx < len; idx++)
		{
			value[idx] = static_cast<double>(pos[currentByte]);
			currentByte += sizeof(double);
		}
	}
	else
	{
		CRASH("Deserialize Error");
	}
}
