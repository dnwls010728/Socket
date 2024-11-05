#include <pch.h>
#include "FileHelper.h"

#include <filesystem>

namespace fs = std::filesystem;

FileHelper::FileHelper()
{
}

std::wstring FileHelper::GetCleanFilename(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.filename().wstring();
}

std::wstring FileHelper::GetBaseFilename(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.stem().wstring();
}

std::wstring FileHelper::GetPath(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.parent_path().wstring();
}
