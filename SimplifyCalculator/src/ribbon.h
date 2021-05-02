#ifndef _RIBBON_H_
#define _RIBBON_H_

#define WM_RIBBON_HEIGHT_CHANGED WM_USER + 10
#define WM_RIBBON_COMMAND WM_USER + 11
#define WM_RIBBON_FONT_CHANGED WM_USER + 12

extern int CreateRibbon(HWND hWnd);
extern void DestroyRibbon();

extern UINT _uRibbonHeight;

#endif /* _RIBBON_H_ */

