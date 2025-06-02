#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <type_traits>
#include <cstring> // for std::memcpy
#include <map>
#include <deque>
#include <queue>
#include <set>

namespace Net {

    // 직렬화/역직렬화 가능 선언
    #define SERIALIZABLE_FIELDS(...) \
    void Serialize(Net::Serializer& serializer) const { \
    serializer.SerializeAll(__VA_ARGS__); \
    } \
    void Deserialize(Net::Serializer& serializer) { \
    serializer.DeserializeAll(__VA_ARGS__); \
    }
    
    // Serializer 확장
    #define SERIALIZER_EXTEND(Type, ...) \
    inline Serializer& operator<<(Serializer& s, const Type& v) { \
    s.SerializeAll(__VA_ARGS__); \
    return s; \
    } \
    inline Serializer& operator>>(Serializer& s, Type& v) { \
    s.DeserializeAll(__VA_ARGS__); \
    return s; \
    }
    
    class Serializer {
    protected:
        std::vector<BYTE> buffer_;
        int offset_;
    public:
        Serializer() : offset_(0) {}
        Serializer(std::vector<BYTE> data) : buffer_(std::move(data)), offset_(0) {}
        virtual ~Serializer() = default;

        // Buffer access
        std::vector<BYTE>& GetData() { return buffer_; }
        const std::vector<BYTE>& GetData() const { return buffer_; }

        // Reset or set buffer
        void SetData(const std::vector<BYTE>& new_data) {
            buffer_ = new_data;
            offset_ = 0;
        }

        // Offset access
        int GetOffset() const { return offset_; }
        void SetOffset(int new_offset) { offset_ = new_offset; }
        void ResetOffset() { offset_ = 0; }
        
        // Member-based SerializeAll / DeserializeAll
        template<typename... Fields>
        void SerializeAll(const Fields&... fields) {
            (( *this << fields), ...);
        }

        template<typename... Fields>
        void DeserializeAll(Fields&... fields) {
            (( *this >> fields), ...);
        }
    };

    // trivially copyable types
    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, Serializer&>
    operator<<(Serializer& s, const T& value) {
        const BYTE* ptr = reinterpret_cast<const BYTE*>(&value);
        s.GetData().insert(s.GetData().end(), ptr, ptr + sizeof(T));
        return s;
    }

    template<typename T>
    std::enable_if_t<std::is_trivially_copyable_v<T>, Serializer&>
    operator>>(Serializer& s, T& value) {
        if (s.GetOffset() + static_cast<int>(sizeof(T)) > static_cast<int>(s.GetData().size()))
            return s;
        value = *reinterpret_cast<const T*>(s.GetData().data() + s.GetOffset());
        s.SetOffset(s.GetOffset() + static_cast<int>(sizeof(T)));
        return s;
    }

    // std::string
    inline Serializer& operator<<(Serializer& s, const std::string& value) {
        int len = static_cast<int>(value.length());
        s << len;
        s.GetData().insert(s.GetData().end(), value.begin(), value.end());
        s.GetData().push_back('\0');
        return s;
    }

    inline Serializer& operator>>(Serializer& s, std::string& value) {
        int len = 0;
        s >> len;
        if (s.GetOffset() + len + 1 > static_cast<int>(s.GetData().size())) {
            value.clear();
            return s;
        }
        value.assign(reinterpret_cast<const char*>(s.GetData().data() + s.GetOffset()), len);
        s.SetOffset(s.GetOffset() + len + 1);
        return s;
    }

    // std::wstring
    inline Serializer& operator<<(Serializer& s, const std::wstring& value) {
        int length = static_cast<int>(value.length()) + 1; // include null
        s << length;
        const BYTE* ptr = reinterpret_cast<const BYTE*>(value.c_str());
        s.GetData().insert(s.GetData().end(), ptr, ptr + length * sizeof(wchar_t));
        return s;
    }

    inline Serializer& operator>>(Serializer& s, std::wstring& value) {
        int length = 0;
        s >> length;
        if (length <= 0) {
            value.clear();
            return s;
        }
        size_t byte_count = static_cast<size_t>(length) * sizeof(wchar_t);
        if (s.GetOffset() + static_cast<int>(byte_count) > static_cast<int>(s.GetData().size())) {
            value.clear();
            return s;
        }
        value.resize(length - 1);
        std::memcpy(value.data(), s.GetData().data() + s.GetOffset(), (length - 1) * sizeof(wchar_t));
        s.SetOffset(s.GetOffset() + static_cast<int>(byte_count));
        return s;
    }

    // std::vector<T>
    template<typename T>
    Serializer& operator<<(Serializer& s, const std::vector<T>& vec) {
        int count = static_cast<int>(vec.size());
        s << count;
        for (const auto& item : vec) s << item;
        return s;
    }

    template<typename T>
    Serializer& operator>>(Serializer& s, std::vector<T>& vec) {
        int count = 0;
        s >> count;
        vec.clear();
        vec.reserve(count);
        for (int i = 0; i < count; ++i) {
            T item;
            s >> item;
            vec.push_back(std::move(item));
        }
        return s;
    }

    // std::deque<T>
    template<typename T>
    Serializer& operator<<(Serializer& s, const std::deque<T>& dq) {
        int count = static_cast<int>(dq.size());
        s << count;
        for (const auto& item : dq) s << item;
        return s;
    }

    template<typename T>
    Serializer& operator>>(Serializer& s, std::deque<T>& dq) {
        int count = 0;
        s >> count;
        dq.clear();
        for (int i = 0; i < count; ++i) {
            T item;
            s >> item;
            dq.push_back(std::move(item));
        }
        return s;
    }

    // std::queue<T>
    template<typename T>
    Serializer& operator<<(Serializer& s, std::queue<T> q) {
        int count = static_cast<int>(q.size());
        s << count;
        while (!q.empty()) {
            s << q.front();
            q.pop();
        }
        return s;
    }

    template<typename T>
    Serializer& operator>>(Serializer& s, std::queue<T>& q) {
        int count = 0;
        s >> count;
        for (int i = 0; i < count; ++i) {
            T item;
            s >> item;
            q.push(std::move(item));
        }
        return s;
    }

    // std::set<T>
    template<typename T>
    Serializer& operator<<(Serializer& s, const std::set<T>& st) {
        int count = static_cast<int>(st.size());
        s << count;
        for (const auto& item : st) s << item;
        return s;
    }

    template<typename T>
    Serializer& operator>>(Serializer& s, std::set<T>& st) {
        int count = 0;
        s >> count;
        st.clear();
        for (int i = 0; i < count; ++i) {
            T item;
            s >> item;
            st.insert(std::move(item));
        }
        return s;
    }

    // std::map<Key,T>
    template<typename K, typename V>
    Serializer& operator<<(Serializer& s, const std::map<K, V>& m) {
        int count = static_cast<int>(m.size());
        s << count;
        for (const auto& kv : m) {
            s << kv.first;
            s << kv.second;
        }
        return s;
    }

    template<typename K, typename V>
    Serializer& operator>>(Serializer& s, std::map<K, V>& m) {
        int count = 0;
        s >> count;
        m.clear();
        for (int i = 0; i < count; ++i) {
            K key;
            V val;
            s >> key;
            s >> val;
            m.emplace(std::move(key), std::move(val));
        }
        return s;
    }

} // namespace Net
