#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <objbase.h>
#include <UIRibbon.h>
#include <initguid.h>
#include <propvarutil.h>
#include "strsafe.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


#ifdef _DEBUG
#include <crtdbg.h>
#endif

#define WM_PANEL_REPAINT WM_USER + 1
#define WM_PANEL_SIZE_CHANGED WM_USER + 2

#define WM_RIBBON_HEIGHT_CHANGED WM_USER + 10
#define WM_RIBBON_FONT_CHANGED WM_USER + 11
#define WM_RIBBON_COMMAND WM_USER + 12



#endif /* PLATFORM_H_ */

