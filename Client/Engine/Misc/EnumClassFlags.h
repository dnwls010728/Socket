#pragma once

#define ENUM_CLASS_FLAGS(enum_class) \
    inline constexpr enum_class operator|(enum_class lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        return static_cast<enum_class>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
    } \
    \
    inline constexpr enum_class operator&(enum_class lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        return static_cast<enum_class>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
    } \
    \
    inline constexpr enum_class operator^(enum_class lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        return static_cast<enum_class>(static_cast<T>(lhs) ^ static_cast<T>(rhs)); \
    } \
    \
    inline constexpr enum_class operator~(enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        return static_cast<enum_class>(~static_cast<T>(rhs)); \
    } \
    \
    inline enum_class& operator|=(enum_class& lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        lhs = static_cast<enum_class>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
        return lhs; \
    } \
    \
    inline enum_class& operator&=(enum_class& lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        lhs = static_cast<enum_class>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
        return lhs; \
    } \
    \
    inline enum_class& operator^=(enum_class& lhs, enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        lhs = static_cast<enum_class>(static_cast<T>(lhs) ^ static_cast<T>(rhs)); \
        return lhs; \
    } \
    \
    inline bool operator!(enum_class rhs) \
    { \
        using T = std::underlying_type_t<enum_class>; \
        return !static_cast<T>(rhs); \
    }

template <typename T>
constexpr bool EnumHasAllFlags(T flags, T contains)
{
    using UnderlyingType = std::underlying_type_t<T>;
    return (static_cast<UnderlyingType>(flags) & static_cast<UnderlyingType>(contains)) == static_cast<UnderlyingType>(contains);
}

template <typename T>
constexpr bool EnumHasAnyFlags(T flags, T contains)
{
    using UnderlyingType = std::underlying_type_t<T>;
    return (static_cast<UnderlyingType>(flags) & static_cast<UnderlyingType>(contains)) != 0;
}
