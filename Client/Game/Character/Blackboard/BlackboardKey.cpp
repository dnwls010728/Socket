#include "pch.h"
#include "BlackboardKey.h"

int Blackboard::ComputeFNV1aHash(const std::wstring& key)
{
    Type::uint32 hash = 2166136261;
    for (const auto& c : key)
    {
        hash = (hash ^ c) * 16777619;
    }

    return static_cast<int>(hash);
}

Blackboard::BlackboardKey::BlackboardKey() :
    name(L""),
    hashed_key(0)
{
}

Blackboard::BlackboardKey::BlackboardKey(const std::wstring& kName) :
    name(kName),
    hashed_key(ComputeFNV1aHash(kName))
{
}

bool Blackboard::BlackboardKey::operator==(const BlackboardKey& kOther) const
{
    return hashed_key == kOther.hashed_key;
}

bool Blackboard::BlackboardKey::operator!=(const BlackboardKey& kOther) const
{
    return !(*this == kOther);
}

int Blackboard::BlackboardKey::GetHashCode() const
{
    return hashed_key;
}

size_t Blackboard::BlackboardKeyHasher::operator()(const BlackboardKey& key) const
{
    return key.GetHashCode();
}
