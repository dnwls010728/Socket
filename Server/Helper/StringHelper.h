#pragma once
#include <string>

namespace StringHelper
{
    std::string ToString(const std::wstring& w_str);
    std::wstring ToWideString(const std::string& str);
    
};
