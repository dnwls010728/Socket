#pragma once    
#include <vector>
#include <string>
#include <Windows.h>

namespace Net {

    
    // 직렬화/역직렬화 멤버 설정. 팩토리 함수를  SerializerClass혹은 SerializerClass의 자식으로 지정하지 않으면 오류 발생. 
#define SERIALIZABLE_FIELDS(SerializerClass, ...)                                           \
    void Serialize(Net::Serializer& serializer) const {                                     \
        SerializerClass* serializer_class = static_cast<SerializerClass*>(&serializer);     \
        serializer_class->SerializeAll(__VA_ARGS__);                                        \
    }                                                                                       \
    void Deserialize(Net::Serializer& serializer) {                                         \
       SerializerClass* serializer_class = static_cast<SerializerClass*>(&serializer);      \
        serializer_class->DeserializeAll(__VA_ARGS__);                                      \
    }
    

#define EXTEND_SERIALIZER()                             \
    using Serializer::operator<<;                       \
    using Serializer::operator>>;                       \
    template<typename... Fields>                        \
    void SerializeAll(Fields&&... fields) {             \
        ((*this << fields), ...);                       \
    }                                                   \
    template<typename... Fields>                        \
    void DeserializeAll(Fields&&... fields) {           \
        ((*this >> fields), ...);                       \
        this->offset_ = 0;                              \
    }                                                   \
    template<typename T>                                \
    CustomSerializer& operator>>(std::vector<T>& vec) { \
        int size = 0;                                   \
        *this >> size;                                  \
        vec.clear();                                    \
        vec.reserve(size);                              \
        for (int i = 0; i < size; ++i) {                \
            T element;                                  \
            *this >> element;                           \
            vec.push_back(std::move(element));          \
        }                                               \
        return *this;                                   \
    }                                                   \
    template<typename T>                                \
    CustomSerializer& operator<<(const std::vector<T>& vec) { \
        int size = static_cast<int>(vec.size());        \
        *this << size;                                  \
        for (const T& element : vec) {                  \
            *this << element;                           \
        }                                               \
        return *this;                                   \
    }


    class Serializer
    {
    protected:
        std::vector<BYTE> buffer_;
        int offset_;
    public:
        Serializer() : offset_(0) {}
        Serializer(std::vector<BYTE> data) : buffer_(data), offset_(0) {}
        virtual ~Serializer() {}
        const std::vector<BYTE> GetData() {
            return buffer_;
        }

        void SetData(std::vector<BYTE> new_data) {
            buffer_ = new_data;
            offset_ = 0;
        }

        template<typename T>
        std::enable_if_t<std::is_trivially_copyable_v<T>, Serializer&>
        operator<<(const T& value) {
            buffer_.insert(buffer_.end(),
                reinterpret_cast<const char*>(&value),
                reinterpret_cast<const char*>(&value) + sizeof(T));
            return *this;
        }

        template<typename T>
        std::enable_if_t<std::is_trivially_copyable_v<T>, Serializer&>
        operator>>(T& value) {
            if (offset_ + sizeof(T) > buffer_.size())
                return *this;
            value = *((T*)(buffer_.data() + offset_));
            offset_ += sizeof(T);
            return *this;
        }

        Serializer& operator<<(const std::string& value) {
            int length = static_cast<int>(value.length());
            *this << length;
            buffer_.insert(buffer_.end(), value.begin(), value.end());
            buffer_.push_back('\0');
            return *this;
        }

        Serializer& operator>>(std::string& value) {
            int length = 0;
            *this >> length;
            if (offset_ + length > buffer_.size())
                return *this;

            value.assign(buffer_.begin() + offset_, buffer_.begin() + offset_ + length);
            offset_ += length;
            return *this;
        }

        Serializer& operator<<(const std::wstring& value) {
            int length = static_cast<int>(value.length()) + 1;
            *this << length;
            buffer_.insert(buffer_.end(),
                reinterpret_cast<const BYTE*>(value.c_str()),
                reinterpret_cast<const BYTE*>(value.c_str()) + length * sizeof(wchar_t));
            return *this;
        }

        Serializer& operator>>(std::wstring& value) {
            int length = 0;
            *this >> length;

            if (length <= 0) {
                value.clear();
                return *this;
            }

            size_t byte_count = length * sizeof(wchar_t);
            if (offset_ + byte_count > buffer_.size())
                return *this;

            value.assign(reinterpret_cast<wchar_t*>(buffer_.data() + offset_), length - 1);
            offset_ += static_cast<int>(byte_count);
            return *this;
        }

        template<typename T>
        Serializer& operator<<(const std::vector<T>& vec) {
            int size = static_cast<int>(vec.size());
            *this << size;
            for (const T& element : vec) {
                *this << element;
            }
            return *this;
        }

        template<typename T>
        Serializer& operator>>(std::vector<T>& vec) {
            int size = 0;
            *this >> size;
            vec.clear();
            vec.reserve(size);
            for (int i = 0; i < size; ++i) {
                T element;
                *this >> element;
                vec.push_back(element);
            }
            return *this;
        }

        template<typename... Fields>
        void SerializeAll(Fields&&... fields) {
            ((*this << fields), ...);
        }
        template<typename... Fields>
        void DeserializeAll(Fields&&... fields) {
            ((*this >> fields), ...);
            this->offset_ = 0;
        }
    };


    
}   // namespace Net