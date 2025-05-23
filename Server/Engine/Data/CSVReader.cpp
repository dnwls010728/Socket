#include "pch.h"
#include "CSVReader.h"

#include <sstream>

CSVReader::CSVReader()
{
}

void CSVReader::Split(const std::wstring& kStr, wchar_t delimiter, std::vector<std::wstring>& out)
{
    std::wistringstream stream(kStr);
    std::wstring buffer;

    while (std::getline(stream, buffer, delimiter))
    {
        out.push_back(buffer);
    }
}
