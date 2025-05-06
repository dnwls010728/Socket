#pragma once
#include <iostream>

#define SAFE_RELEASE(p) \
    do { \
        if (p) \
        { \
            delete p; \
            p = nullptr; \
        } \
    } while (0)

#define CHECK(condition) \
    do { \
        if (!(condition)) \
        { \
            std::cerr << "Assertion failed: (" #condition "), function " << __FUNCTION__ \
                << ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while (0)

#define CHECK_IF(condition, format, ...) \
    do { \
        if (!(condition)) \
        { \
            wchar_t buffer[256]; \
            swprintf(buffer, sizeof(buffer), format, __VA_ARGS__); \
            std::wcerr << L"Assertion failed: (" #condition L"), function " << __FUNCTIONW__ \
                << L", file " << __FILEW__ << L", line " << __LINE__ << L".\n" \
                << L"Message: " << buffer << std::endl; \
            std::abort(); \
        } \
    } while (0)

#define SHADER_CLASS_HELPER(class_name) \
    public: \
        std::shared_ptr<class_name> GetSharedThis() \
        { \
            return std::dynamic_pointer_cast<class_name>(shared_from_this()); \
        } \
    private:

#define GENERATED_BODY(class_name, ...) \
    RTTR_ENABLE(__VA_ARGS__) \
    RTTR_REGISTRATION_FRIEND \
    \
    public: \
        FORCEINLINE static rttr::type StaticClass() \
        { \
            return rttr::type::get<class_name>(); \
        }
