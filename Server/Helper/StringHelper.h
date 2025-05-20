#pragma once
#include <string>

namespace StringHelper
{
    std::wstring UTF8ToUTF16(const std::string& utf8);
    std::string UTF16ToUTF8(const std::wstring& utf16);
};
