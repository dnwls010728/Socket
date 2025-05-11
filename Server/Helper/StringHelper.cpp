#include "pch.h"
#include "StringHelper.h"

std::string StringHelper::ToString(const std::wstring& w_str)
{
    return std::string(w_str.begin(), w_str.end());
}

std::wstring StringHelper::ToWideString(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}
