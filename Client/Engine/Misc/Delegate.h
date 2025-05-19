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

    // void Execute(Args&&... args) const {
    //     for (const auto& func : functions_) {
    //         func(std::forward<Args>(args)...);
    //     }
    // }

    void Execute(Args&... args) const {
        for (const auto& func : functions_) {
            func(args...);
        }
    }

    void ExecuteIfBound(Args&... args) const
    {
        if (IsBound()) Execute(args...);
    }

    void RemoveAll() {
        functions_.clear();
    }

    template<typename F>
    void Remove(F&& func) {
        RemoveImpl(GetFunctionAddress(std::forward<F>(func)));
    }
    
    void Remove(Ret(*func)(Args...)) {
        RemoveImpl(GetFunctionAddress(func));
    }

    template<typename M>
    void Remove(M* target, Ret(M::*func)(Args...)) {
        FunctionType temp(target, func);
        RemoveImpl(temp.GetAddr());
    }

    template<typename M>
    void Remove(M* target, Ret(M::*func)(Args...) const) {
        FunctionType temp(target, func);
        RemoveImpl(temp.GetAddr());
    }

    // template<typename F>
    // bool IsBound(F&& func) const {
    //     return IsBoundImpl(GetFunctionAddress(std::forward<F>(func)));
    // }

    /**
     * 이 델리게이트가 바인딩된 함수가 있는지 확인합니다.
     * @return 어떤 함수가 바인딩되어 있으면 true, 아니면 false를 반환합니다.
     */
    bool IsBound()
    {
        return !functions_.empty();
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

    // bool IsBoundImpl(std::uintptr_t addr) const {
    //     return std::any_of(functions_.begin(), functions_.end(),
    //         [addr](const auto& func) { return func.GetAddr() == addr; });
    // }
};