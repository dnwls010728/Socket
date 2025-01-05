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
        bool TryGetValue(const BlackboardKey& kKey, T& kValue) const;

        template<typename T>
        void SetValue(const BlackboardKey& kKey, const T& kValue);
        
        BlackboardKey GetOrRegisterKey(const std::wstring& kName);

        bool ContainsKey(const BlackboardKey& kKey) const;

        void Remove(const BlackboardKey& kKey);

    private:
        std::unordered_map<std::wstring, BlackboardKey> key_registry_;
        std::unordered_map<BlackboardKey, std::any, BlackboardKeyHasher> entries_;
    
    };

    template <typename T>
    bool Blackboard::TryGetValue(const BlackboardKey& kKey, T& kValue) const
    {
        auto it = entries_.find(kKey);
        if (it == entries_.end() || it->second.type() != typeid(T))
        {
            kValue = T();
            return false;
        }

        try
        {
            kValue = std::any_cast<T>(it->second);
            return true;
        }
        catch (const std::bad_any_cast&)
        {
            kValue = T();
            return false;
        }
    }

    template <typename T>
    void Blackboard::SetValue(const BlackboardKey& kKey, const T& kValue)
    {
        BlackboardEntry<T> entry(kKey, kValue);
        entries_[kKey] = entry;
    }
}
