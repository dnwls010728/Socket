#include "pch.h"
#include "Blackboard.h"

Blackboard::Blackboard::Blackboard() :
    key_registry_(),
    entries_()
{
}

Blackboard::BlackboardKey Blackboard::Blackboard::GetOrRegisterKey(const std::wstring& kName)
{
    auto [it, inserted] = key_registry_.emplace(kName, BlackboardKey(kName));
    return it->second;
}

bool Blackboard::Blackboard::ContainsKey(const BlackboardKey& kKey) const
{
    return entries_.contains(kKey);
}

void Blackboard::Blackboard::Remove(const BlackboardKey& kKey)
{
    entries_.erase(kKey);
}
