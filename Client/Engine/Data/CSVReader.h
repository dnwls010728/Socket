#pragma once
#include <fstream>
#include <vector>

#include "Singleton.h"
#include "rttr/type.h"

class CSVReader : public Singleton<CSVReader>
{
public:
    CSVReader();
    virtual ~CSVReader() override = default;

    template <typename T>
    bool Parse(const std::wstring& kPath, std::vector<T>& out);

private:
    void Split(const std::wstring& kStr, wchar_t delimiter, std::vector<std::wstring>& out);
    
};

template <typename T>
bool CSVReader::Parse(const std::wstring& kPath, std::vector<T>& out)
{
    std::wifstream file(kPath);
    if (!file.is_open()) return false;

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
        for (int i = 0; i < headers.size(); i++)
        {
            rttr::type type = rttr::type::get<T>();

            std::string name = std::string(headers[i].begin(), headers[i].end());
            rttr::property prop = type.get_property(name);
            if (prop.is_valid())
            {
                rttr::type prop_type = prop.get_type();
                if (prop_type == rttr::type::get<int>())
                {
                    int value = std::wcstol(values[i].c_str(), nullptr, 10);
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
