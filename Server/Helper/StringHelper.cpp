#include "pch.h"
#include "StringHelper.h"

#include <windows.h>

std::wstring StringHelper::UTF8ToUTF16(const std::string& utf8)
{
    if (utf8.empty()) return {};
    int length = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    std::wstring utf16(length, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), utf16.data(), length);
    return utf16;
}

std::string StringHelper::UTF16ToUTF8(const std::wstring& utf16)
{
    if (utf16.empty()) return {};
    int length = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()), nullptr, 0, nullptr, nullptr);
    std::string utf8(length, '\0');
    WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()), utf8.data(), length, nullptr, nullptr);
    return utf8;
}
