#pragma once
#include "Function.h"
#include <vector>
#include <type_traits>

template<typename Signature>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)> {
public:
    using FunctionType = Function<Ret(Args...)>;

    template<typename F, typename = std::enable_if_t<!std::is_same_v<FunctionType, std::decay_t<F>>>>
    void Add(F&& func) {
        functions_.emplace_back(std::forward<F>(func));
    }

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void Add(M* target, Ret(M::*func)(Args...)) {
        functions_.emplace_back(target, func);
    }

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void Add(M* target, Ret(M::*func)(Args...) const) {
        functions_.emplace_back(target, func);
    }

    void Add(Ret(*func)(Args...)) {
        functions_.emplace_back(func);
    }

    void Execute(Args&&... args) const {
        for (const auto& func : functions_) {
            func(std::forward<Args>(args)...);
        }
    }

    void RemoveAll() {
        functions_.clear();
    }

    template<typename F>
    void Remove(F&& func) {
        RemoveImpl(GetFunctionAddress(std::forward<F>(func)));
    }

    template<typename F>
    bool IsBound(F&& func) const {
        return IsBoundImpl(GetFunctionAddress(std::forward<F>(func)));
    }

private:
    std::vector<FunctionType> functions_;

    template<typename F>
    static std::uintptr_t GetFunctionAddress(F&& func) {
        if constexpr (std::is_member_function_pointer_v<std::decay_t<F>>) {
            return reinterpret_cast<std::uintptr_t&>(func);
        } else {
            std::uintptr_t addr = 0;
            std::memcpy(&addr, &func, sizeof(addr));
            return addr;
        }
    }

    void RemoveImpl(std::uintptr_t addr) {
        auto it = std::remove_if(functions_.begin(), functions_.end(),
            [addr](const auto& func) { return func.GetAddr() == addr; });
        functions_.erase(it, functions_.end());
    }

    bool IsBoundImpl(std::uintptr_t addr) const {
        return std::any_of(functions_.begin(), functions_.end(),
            [addr](const auto& func) { return func.GetAddr() == addr; });
    }
};