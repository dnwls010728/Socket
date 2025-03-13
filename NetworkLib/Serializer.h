#pragma once    
#include <vector>
#include <string>
#include <Windows.h>

namespace Net {

    class Serializer
    {
    private:
        std::vector<BYTE> buffer_;
        int offset_;
    public:
        Serializer() : offset_(0) {}
        Serializer(std::vector<BYTE> data) : buffer_(data), offset_(0) {}

        const std::vector<BYTE> GetData() {
            return buffer_;
        }

        void SetData(std::vector<BYTE> new_data) {
            buffer_ = new_data;
            offset_ = 0;
        }

        template<typename T>
        void operator<<(T& value) {
            buffer_.insert(buffer_.end(),
                reinterpret_cast<const char*>(&value),
                reinterpret_cast<const char*>(&value) + sizeof(T));
        }

        template<typename T>
        void operator>>(T& value) {
            if (offset_ + sizeof(T) > buffer_.size())
                return;
            value = *((T*)(buffer_.data() + offset_));
            offset_ += sizeof(T);
        }

        template<typename SerializerT, typename... Fields>
        static void SerializeAll(SerializerT& serializer, Fields&... fields) {
            ((serializer << fields), ...);
        }

        template<typename SerializerT, typename... Fields>
        static void DeserializeAll(SerializerT& serializer, Fields&... fields) {
            ((serializer >> fields), ...);
            serializer.offset_ = 0;
        }
    };

    template<>
    inline void Serializer::operator<<(const std::string& value) {
        int length = static_cast<int>(value.length());
        *this << length;
        buffer_.insert(buffer_.end(), value.begin(), value.end());
        buffer_.push_back('\0');
    }

    template<>
    inline void Serializer::operator>>(std::string& value) {
        int length = 0;
        *this >> length;
        if (offset_ + length > buffer_.size())
            return;

        value.assign(buffer_.begin() + offset_, buffer_.begin() + offset_ + length);
        offset_ += length;
    }
}   // namespace Net