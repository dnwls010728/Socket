#pragma once

#define DEFINE_BIND_OVERLOADS(Variable, FunctionName, ReturnType, ...) \
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<ReturnType(__VA_ARGS__)>, std::decay_t<F>>>> \
    FORCEINLINE void FunctionName(F&& func) { Variable = std::forward<F>(func); } \
    \
    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>> \
    FORCEINLINE void FunctionName(M* target, ReturnType(M::*func)(__VA_ARGS__)) { Variable = { target, func }; } \
    \
    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>> \
    FORCEINLINE void FunctionName(M* target, ReturnType(M::*func)(__VA_ARGS__) const) { Variable = { target, func }; } \
    \
    FORCEINLINE void FunctionName(ReturnType(*func)(__VA_ARGS__)) { Variable = func; }
