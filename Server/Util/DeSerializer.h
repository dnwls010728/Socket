#pragma once
class DeSerializer
{
public:
	template<typename T>
	static T Deserialize(BYTE* pos, int& currentByte);
};

template<typename T>
inline T DeSerializer::Deserialize(BYTE* pos, int& currentByte)
{
	//TODO: Deserialize 중 패킷 불일치 발생 시 로직 처리 
	uint8_t spliter = pos[currentByte];
	currentByte++;
	switch (currentByte) {
		//uint8_t
		case 1: 
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//uint16_t
		case 2:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//uint32_t
		case 3:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//int32_t
		case 4:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//uint64_t
		case 5:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//int64_t
		case 6:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//string
		case 7:
		{
			uint8_t strLen = static_cast<uint8_t>(pos[currentByte]);
			currentByte++;
			T str(reinterpret_cast<const char*>(pos + currentByte));
			currentByte += str.length();
			return str;
			break;
		}
		//uint16_t*
		case 8:
		{
			uint8_t len = pos[currentByte];
			currentByte++;
			T* t = new T[len];
			for (uint8_t idx = 0; idx < len; idx++) {
				t[idx] = static_cast<T>(pos[currentByte]);
				currentByte += sizeof(T);
			}
			return t;
			break;
		}
		//uint32_t*
		case 9:
		{
			uint8_t len = pos[currentByte];
			currentByte++;
			T* t = new T[len];
			for (uint8_t idx = 0; idx < len; idx++) {
				t[idx] = static_cast<T>(pos[currentByte]);
				currentByte += sizeof(T);
			}
			return t;
			break;
		}
		//int64_t*
		case 10:
		{
			uint8_t len = pos[currentByte];
			currentByte++;
			T* t = new T[len];
			for (uint8_t idx = 0; idx < len; idx++) {
				t[idx] = static_cast<T>(pos[currentByte]);
				currentByte += sizeof(T);
			}
			return t;
			break;
		}
		//double
		case 11:
		{
			T value = static_cast<T>(pos[currentByte]);
			currentByte += sizeof(T);
			return value;
			break;
		}
		//double *
		case 12:
		{
			uint8_t len = pos[currentByte];
			currentByte++;
			T* t = new T[len];
			for (uint8_t idx = 0; idx < len; idx++) {
				t[idx] = static_cast<T>(pos[currentByte]);
				currentByte += sizeof(T);
			}
			return t;
			break;
		}
	}
}
