#pragma once

#define SAFE_RELEASE(p) \
    do { \
        if (p) \
        { \
            delete p; \
            p = nullptr; \
        } \
    } while (0)
