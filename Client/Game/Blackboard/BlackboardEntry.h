#pragma once
#include <typeindex>

#include "BlackboardKey.h"

namespace Blackboard
{
    class BlackboardEntryBase
    {
    public:
        virtual ~BlackboardEntryBase() = default;

    protected:
        BlackboardEntryBase() = default;
        
    };
    
    template <typename T>
    class BlackboardEntry : public BlackboardEntryBase
    {
    public:
        BlackboardEntry(const BlackboardKey& key, T value);
        virtual ~BlackboardEntry() override = default;

        FORCEINLINE const BlackboardKey& GetKey() const { return key_; }
        FORCEINLINE T GetValue() const { return value_; }
        FORCEINLINE std::type_index GetType() const { return type_; }
        FORCEINLINE Type::uint64 GetHashCode() const { return key_.GetHashCode(); }

    private:
        BlackboardKey key_;
        T value_;
        std::type_index type_;
    
    };

    template <typename T>
    BlackboardEntry<T>::BlackboardEntry(const BlackboardKey& key, T value) :
        key_(key),
        value_(value),
        type_(typeid(T))
    {
    }
}
