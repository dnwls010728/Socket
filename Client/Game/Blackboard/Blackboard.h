#pragma once
#include <any>

#include "BlackboardEntry.h"
#include "BlackboardKey.h"

namespace Blackboard
{
    class Blackboard
    {
    public:
        Blackboard();
        ~Blackboard() = default;

        template<typename T>
        bool TryGetValue(const BlackboardKey& key, T& out_value) const;

        template <typename T>
        void SetValue(const BlackboardKey& key, const T& value);

        BlackboardKey FindOrAdd(const std::wstring& name);

        bool Contains(const BlackboardKey& key) const;

        void Remove(const BlackboardKey& key);

    private:
        std::unordered_map<std::wstring, BlackboardKey> keys_;
        std::unordered_map<BlackboardKey, std::unique_ptr<BlackboardEntryBase>, BlackboardKeyHasher> entries_;
    
    };

    template <typename T>
    bool Blackboard::TryGetValue(const BlackboardKey& key, T& out_value) const
    {
        const auto it = entries_.find(key);
        if (it != entries_.end())
        {
            BlackboardEntry<T>* entry = dynamic_cast<BlackboardEntry<T>*>(it->second.get());
            if (entry)
            {
                out_value = entry->GetValue();
                return true;
            }
        }

        out_value = T();
        return false;
    }

    template <typename T>
    void Blackboard::SetValue(const BlackboardKey& key, const T& value)
    {
        std::unique_ptr<BlackboardEntry<T>> entry = std::make_unique<BlackboardEntry<T>>(key, value);
        entries_[key] = std::move(entry);
    }
}
