// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//공통 정적 라이브러리
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\CommonDLL.lib");
#else
#pragma comment(lib,"..\\x64\\Release\\CommonDLL.lib");
#endif

//서버 관련 정적 변수 정의 헤더
#include "SocketCore/Util/GlobalFreeManager.h"