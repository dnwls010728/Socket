#include "pch.h"
#include "BlackboardKey.h"

Type::uint64 Blackboard::ComputeFNV1aHash(const std::wstring& str)
{
    Type::uint64 hash = 14695981039346656037ULL;
    for (const auto& c : str)
    {
        hash = (hash ^ static_cast<Type::uint64>(c)) * 1099511628211ULL;
    }

    return hash;
}
