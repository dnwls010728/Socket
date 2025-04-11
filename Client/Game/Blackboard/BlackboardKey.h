#pragma once

namespace Blackboard
{
    Type::uint64 ComputeFNV1aHash(const std::wstring& str);

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

        FORCEINLINE bool operator==(const BlackboardKey& other) const { return hashed_key_ == other.hashed_key_; }
        FORCEINLINE bool operator!=(const BlackboardKey& other) const { return !(*this == other); }

        FORCEINLINE std::wstring ToString() const { return name_; }
        FORCEINLINE Type::uint64 GetHashCode() const { return hashed_key_; }

    private:
        std::wstring name_;
        Type::uint64 hashed_key_;
        
    };

    struct BlackboardKeyHasher
    {
        FORCEINLINE Type::uint64 operator()(const BlackboardKey& key) const { return key.GetHashCode(); }
        
    };
}
