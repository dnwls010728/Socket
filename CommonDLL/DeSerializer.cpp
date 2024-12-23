#include "pch.h"
#include "DeSerializer.h"
DeSerializer* GDeSerializer = new DeSerializer();

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint8_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 1)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(uint8_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint16_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 2)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
			
		currentByte += sizeof(uint16_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint32_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 3)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(uint32_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, int32_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 4)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		
		currentByte += sizeof(int32_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, uint64_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 5)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(uint64_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, int64_t& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 6)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(int64_t);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, std::string& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 7)
	{
		currentByte++;
		value = std::string(reinterpret_cast<const char*>(pos + currentByte));
		currentByte += static_cast<int>(value.length()+1);
	}
	else
	{
		CRASH("Deserialize Error")
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
			
			std::memcpy(&value[idx],pos+currentByte,sizeof(value));
			currentByte += sizeof(uint16_t);
		}
	}
	else
	{
		CRASH("Deserialize Error")
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
		//이때 value는 쓰레기 값
		//HeapInitialize(&value,len);
		
		
		for (uint8_t idx = 0; idx < len; idx++)
		{
			std::memcpy(value+idx,pos+currentByte,sizeof(value));
			currentByte += sizeof(uint32_t);
		}
	}
	else
	{
		CRASH("Deserialize Error")
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
		//HeapInitialize(&value,len);
		for (uint8_t idx = 0; idx < len; idx++)
		{
			std::memcpy(value+idx,pos+currentByte,sizeof(int64_t));
			currentByte += sizeof(int64_t);
		}
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, double& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 11)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(double);
	}
	else
	{
		CRASH("Deserialize Error")
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
		//HeapInitialize(&value,len);
		for (uint8_t idx = 0; idx < len; idx++)
		{
			std::memcpy(value+idx,pos+currentByte,sizeof(double));
			currentByte += sizeof(double);
		}
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, float& value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 13)
	{
		std::memcpy(&value,pos+currentByte,sizeof(value));
		currentByte += sizeof(float);
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, float* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if (spliter == 14)
	{
		uint8_t len = pos[currentByte];
		currentByte++;
		//HeapInitialize(&value,len);
		for (uint8_t idx = 0; idx < len; idx++)
		{
			std::memcpy(value+idx,pos+currentByte,sizeof(float));
			currentByte += sizeof(float);
		}
	}
	else
	{
		CRASH("Deserialize Error")
	}
}

void DeSerializer::Deserialize(BYTE* pos, int& currentByte, std::string* value)
{
	uint8_t spliter = pos[currentByte];
	currentByte++;
	if(spliter == 15)
	{
		uint8_t len =  pos[currentByte];
		currentByte++;
		//HeapInitialize(&value,len);
		for(uint8_t idx = 0; idx < len; idx++)
		{
			currentByte++;
			value[idx] = std::string(reinterpret_cast<const char*>(pos + currentByte));
			currentByte += static_cast<int>(value[idx].length()+1);
		}
	}
}
