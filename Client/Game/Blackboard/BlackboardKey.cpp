#include "pch.h"
#include "BlackboardKey.h"

uint64_t Blackboard::ComputeFNV1aHash(const std::wstring& str)
{
    uint64_t hash = 14695981039346656037ULL;
    for (const auto& c : str)
    {
        hash = (hash ^ static_cast<uint64_t>(c)) * 1099511628211ULL;
    }

    return hash;
}
