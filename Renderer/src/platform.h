#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "strsafe.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>


#ifdef _DEBUG
#include <crtdbg.h>
#endif

#ifdef __linux__
#include "linux_def.h"
#endif


#endif /* _PLATFORM_H_*/
