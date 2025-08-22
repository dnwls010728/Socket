#pragma once

#define DEFINE_BIND_OVERLOADS(Variable, FunctionName, ReturnType, ...) \
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<ReturnType(__VA_ARGS__)>, std::decay_t<F>>>> \
    inline void FunctionName(F&& func) { Variable = std::forward<F>(func); } \
    \
    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>> \
    inline void FunctionName(M* target, ReturnType(M::*func)(__VA_ARGS__)) { Variable = { target, func }; } \
    \
    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>> \
    inline void FunctionName(M* target, ReturnType(M::*func)(__VA_ARGS__) const) { Variable = { target, func }; } \
    \
    inline void FunctionName(ReturnType(*func)(__VA_ARGS__)) { Variable = func; }
