#include <pch.h>
#include "FileHelper.h"

#include <filesystem>

namespace fs = std::filesystem;

FileHelper::FileHelper()
{
}

bool FileHelper::IsFileExists(const std::wstring& kPath)
{
    fs::path path = kPath;
    return fs::exists(path);
}

std::wstring FileHelper::GetFilename(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.filename().wstring();
}

std::wstring FileHelper::GetFilenameWithoutExtension(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.stem().wstring();
}

std::wstring FileHelper::GetPath(const std::wstring& kPath)
{
    fs::path path = kPath;
    return path.parent_path().wstring();
}
