#pragma once
#include <codecvt>
#include <fstream>
#include <vector>

#include "rttr/type.h"

class CSVReader
{
public:
    CSVReader();
    ~CSVReader() = default;

    template <typename T>
    static bool Parse(const std::wstring& kPath, std::vector<T>& out);

private:
    static void Split(const std::wstring& kStr, wchar_t delimiter, std::vector<std::wstring>& out);
    
};

template <typename T>
bool CSVReader::Parse(const std::wstring& kPath, std::vector<T>& out)
{
    std::wifstream file(L".\\Content\\" + kPath, std::ios::binary);
    if (!file.is_open()) return false;
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    std::wstring line;
    std::vector<std::wstring> headers = {};
    if (std::getline(file, line))
    {
        Split(line, L',', headers);
    }
    
    while (std::getline(file, line))
    {
        std::vector<std::wstring> values = {};
        Split(line, L',', values);

        T data;
        for (int32_t i = 0; i < headers.size(); i++)
        {
            rttr::type type = rttr::type::get<T>();

            std::string name = std::string(headers[i].begin(), headers[i].end());
            rttr::property prop = type.get_property(name);
            if (prop.is_valid())
            {
                rttr::type prop_type = prop.get_type();
                if (prop_type == rttr::type::get<int32_t>())
                {
                    int32_t value = std::wcstol(values[i].c_str(), nullptr, 10);
                    prop.set_value(data, value);
                }
                else if (prop_type == rttr::type::get<float>())
                {
                    float value = std::wcstof(values[i].c_str(), nullptr);
                    prop.set_value(data, value);
                }
                else if (prop_type == rttr::type::get<std::wstring>())
                {
                    prop.set_value(data, values[i]);
                }
            }
        }

        out.push_back(data);
    }

    return true;
}
