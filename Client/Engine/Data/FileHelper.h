#pragma once
#include "Singleton.h"

class FileHelper : public Singleton<FileHelper>
{
public:
    FileHelper();
    virtual ~FileHelper() override = default;

    static std::wstring GetCleanFilename(const std::wstring& kPath);
    static std::wstring GetBaseFilename(const std::wstring& kPath);
    static std::wstring GetPath(const std::wstring& kPath);
    
};
