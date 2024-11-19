#include "pch.h"
#include "RegistryHelper.h"

void RegistryHelper::DeleteAll()
{
    RegDeleteKey(HKEY_CURRENT_USER, L"Software\\Game");
}

void RegistryHelper::DeleteKey(const std::wstring& kKey)
{
    HKEY hKey = nullptr;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
    {
        RegDeleteValue(hKey, kKey.c_str());
    }

    RegCloseKey(hKey);
}

bool RegistryHelper::HasKey(const std::wstring& kKey)
{
    HKEY hKey = nullptr;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hKey, kKey.c_str(), nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return true;
        }
    }

    RegCloseKey(hKey);
    return false;
}

void RegistryHelper::SetInt(const std::wstring& kKey, int value)
{
    HKEY hKey = nullptr;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        RegSetValueEx(hKey, kKey.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(int));
    }
    
    RegCloseKey(hKey);
}

void RegistryHelper::SetFloat(const std::wstring& kKey, float value)
{
    HKEY hKey = nullptr;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        RegSetValueEx(hKey, kKey.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(float));
    }

    RegCloseKey(hKey);
}

void RegistryHelper::SetString(const std::wstring& kKey, const std::wstring& kValue)
{
    HKEY hKey = nullptr;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS)
    {
        RegSetValueEx(hKey, kKey.c_str(), 0, REG_BINARY, reinterpret_cast<const BYTE*>(kValue.c_str()), kValue.size() * sizeof(wchar_t));
    }

    RegCloseKey(hKey);
}

int RegistryHelper::GetInt(const std::wstring& kKey, int kDefaultValue)
{
    HKEY hKey = nullptr;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD type = REG_DWORD;
        DWORD size = sizeof(int);
        int data = 0;
        if (RegQueryValueEx(hKey, kKey.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(&data), &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return data;
        }
    }

    RegCloseKey(hKey);
    return kDefaultValue;
}

float RegistryHelper::GetFloat(const std::wstring& kKey, float kDefaultValue)
{
    HKEY hKey = nullptr;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD type = REG_DWORD;
        DWORD size = sizeof(float);
        float data = 0;
        if (RegQueryValueEx(hKey, kKey.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(&data), &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return data;
        }
    }

    RegCloseKey(hKey);
    return kDefaultValue;
}

std::wstring RegistryHelper::GetString(const std::wstring& kKey, const std::wstring& kDefaultValue)
{
    HKEY hKey = nullptr;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Game", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD type = REG_BINARY;
        DWORD size = 0;
        if (RegQueryValueEx(hKey, kKey.c_str(), nullptr, &type, nullptr, &size) == ERROR_SUCCESS)
        {
            std::wstring data;
            data.resize(size / sizeof(wchar_t));
            if (RegQueryValueEx(hKey, kKey.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(&data[0]), &size) == ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return data;
            }
        }
    }

    RegCloseKey(hKey);
    return kDefaultValue;
}
