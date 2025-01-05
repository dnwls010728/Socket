#pragma once

namespace Blackboard
{
    int ComputeFNV1aHash(const std::wstring& key);
    
    struct BlackboardKey
    {
    public:
        BlackboardKey();
        BlackboardKey(const std::wstring& kName);

        bool operator==(const BlackboardKey& kOther) const;
        bool operator!=(const BlackboardKey& kOther) const;

        int GetHashCode() const;

    private:
        std::wstring name;
        int hashed_key;
    };

    struct BlackboardKeyHasher
    {
        size_t operator()(const BlackboardKey& key) const;
    };
}
