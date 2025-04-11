#include "pch.h"
#include "Blackboard.h"

Blackboard::Blackboard::Blackboard() :
    keys_(),
    entries_()
{
}

Blackboard::BlackboardKey Blackboard::Blackboard::FindOrAdd(const std::wstring& name)
{
    auto [it, inserted] = keys_.emplace(name, BlackboardKey(name));
    return it->second;
}

bool Blackboard::Blackboard::Contains(const BlackboardKey& key) const
{
    return entries_.contains(key);
}

void Blackboard::Blackboard::Remove(const BlackboardKey& key)
{
    entries_.erase(key);
}
