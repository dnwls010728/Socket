#include "pch.h"

#include <cstdarg>
#include <Windows.h>

Logger::Logger()
{
}

void Logger::Print(const wchar_t* message, ...)
{
    va_list args;
    va_start(args, message);

    wchar_t buffer[256];
    vswprintf(buffer, sizeof(buffer), message, args);
    wcscat_s(buffer, L"\n");
    
    OutputDebugString(buffer);
    va_end(args);
}
