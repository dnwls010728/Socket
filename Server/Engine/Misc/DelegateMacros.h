#pragma once
#include "Delegate.h"

#define FUNC_DECLARE_DELEGATE(DelegateName, ReturnType, ...) \
    typedef Delegate<ReturnType(__VA_ARGS__)> DelegateName;

#define DECLARE_DELEGATE(DelegateName, ...) FUNC_DECLARE_DELEGATE(DelegateName, void, __VA_ARGS__)
#define DECLARE_DELEGATE_RetVal(DelegateName, ReturnType, ...) FUNC_DECLARE_DELEGATE(DelegateName, ReturnType, __VA_ARGS__)
