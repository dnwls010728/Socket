#pragma once

/**
 * 스캔코드를 출력합니다.
 */
static constexpr bool kDebugScancode = false;

enum class Scancode : uint32_t
{
    kKeyUnknown = 0,

    kKey0 = 2,
    kKey1 = 3,
    kKey2 = 4,
    kKey3 = 5,
    kKey4 = 6,
    kKey5 = 7,
    kKey6 = 8,
    kKey7 = 9,
    kKey8 = 10,
    kKey9 = 11,

    kKeyLeft = 331,
    kKeyRight = 333,
    kKeyUp = 330,
    kKeyDown = 332,

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
    kKeyTab = 15,
    kKeyBackspace = 14,
    kKeyDelete = 339,
    kKeyHome = 327,
    kKeyEnd = 335,
};
