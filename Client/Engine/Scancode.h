#pragma once

/**
 * 스캔코드를 출력합니다.
 */
static constexpr bool kDebugScancode = true;

enum class Scancode : uint32_t
{
    kKeyUnknown = 0,
    
    kKey1 = 2,
    kKey2 = 3,
    kKey3 = 4,
    kKey4 = 5,
    kKey5 = 6,
    kKey6 = 7,
    kKey7 = 8,
    kKey8 = 9,
    kKey9 = 10,
    kKey0 = 11,

    kKeyLeft = 331,
    kKeyRight = 333,
    kKeyUp = 328,
    kKeyDown = 336,

    kKeyA = 30,
    kKeyB = 48,
    kKeyC = 46,
    kKeyD = 32,
    kKeyE = 18,
    kKeyF = 33,
    kKeyG = 34,
    kKeyH = 35,
    kKeyI = 23,
    kKeyJ = 36,
    kKeyK = 37,
    kKeyL = 38,
    kKeyM = 50,
    kKeyN = 49,
    kKeyO = 24,
    kKeyP = 25,
    kKeyQ = 16,
    kKeyR = 19,
    kKeyS = 31,
    kKeyT = 20,
    kKeyU = 22,
    kKeyV = 47,
    kKeyW = 17,
    kKeyX = 45,
    kKeyY = 21,
    kKeyZ = 44,

    kKeyEnter = 28,
    kKeyEscape = 1,
    kKeyTab = 15,
    kKeyBackspace = 14,
    kKeyDelete = 339,
    kKeyHome = 327,
    kKeyEnd = 335,
};
