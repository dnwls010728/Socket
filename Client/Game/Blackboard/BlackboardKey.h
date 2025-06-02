#pragma once

namespace Blackboard
{
    uint64_t ComputeFNV1aHash(const std::wstring& str);

    struct BlackboardKey
    {
        BlackboardKey() :
            name_(L""),
            hashed_key_(0)
        {
        }
        
        BlackboardKey(const std::wstring& name) :
            name_(name),
            hashed_key_(ComputeFNV1aHash(name))
        {
        }

        FORCEINLINE bool IsValid() const { return hashed_key_ != 0; }

        FORCEINLINE bool operator==(const BlackboardKey& other) const { return hashed_key_ == other.hashed_key_; }
        FORCEINLINE bool operator!=(const BlackboardKey& other) const { return !(*this == other); }

        FORCEINLINE std::wstring ToString() const { return name_; }
        FORCEINLINE uint64_t GetHashCode() const { return hashed_key_; }

    private:
        std::wstring name_;
        uint64_t hashed_key_;
        
    };

    struct BlackboardKeyHasher
    {
        FORCEINLINE uint64_t operator()(const BlackboardKey& key) const { return key.GetHashCode(); }
        
    };
}
