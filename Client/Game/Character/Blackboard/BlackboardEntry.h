#pragma once
#include <typeindex>

#include "BlackboardKey.h"

namespace Blackboard
{
    template<typename T>
    class BlackboardEntry
    {
    public:
        BlackboardEntry(const BlackboardKey& kKey, const T& value);
        ~BlackboardEntry() = default;

        bool operator==(const BlackboardEntry& kOther) const;
        bool operator!=(const BlackboardEntry& kOther) const;

        int GetHashCode() const;

    private:
        friend class Blackboard;
        
        BlackboardKey key_;
        T value_;
        std::type_index value_type_;
    
    };

    template <typename T>
    BlackboardEntry<T>::BlackboardEntry(const BlackboardKey& kKey, const T& value) :
        key_(kKey),
        value_(value),
        value_type_(typeid(T))
    {
    }

    template <typename T>
    bool BlackboardEntry<T>::operator==(const BlackboardEntry& kOther) const
    {
        return key_ == kOther.key_;
    }

    template <typename T>
    bool BlackboardEntry<T>::operator!=(const BlackboardEntry& kOther) const
    {
        return !(*this == kOther);
    }

    template <typename T>
    int BlackboardEntry<T>::GetHashCode() const
    {
        return key_.GetHashCode();
    }
}
