#pragma once
#include "Singleton.h"

class FileHelper : public Singleton<FileHelper>
{
public:
    FileHelper();
    virtual ~FileHelper() override = default;

    static bool IsFileExists(const std::wstring& kPath);
    
    static std::wstring GetFilename(const std::wstring& kPath);
    static std::wstring GetFilenameWithoutExtension(const std::wstring& kPath);
    static std::wstring GetPath(const std::wstring& kPath);
    
};
