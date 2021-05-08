#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"
#include "items.h"

#define WM_PANEL_REPAINT WM_USER + 1
#define WM_PANEL_SIZE_CHANGED WM_USER + 2

typedef struct _Panel Panel;

typedef void (*DrawFunc)(Panel* p, HDC hdc);
typedef void (*PosChangedFunc)(Panel* p);
typedef void (*FontChangedFunc)(Panel* p);
typedef void (*CalcSizeFunc)(Panel* p);

typedef void (*OnKey_LeftArrowFunc)(Panel* p);
typedef void (*OnKey_RightArrowFunc)(Panel* p);
typedef void (*OnChar_DefaultFunc)(Panel* p, wchar_t ch);
typedef void (*OnChar_BackspaceFunc)(Panel* p);
typedef void (*OnChar_ReturnFunc)(Panel* p);

typedef struct _Panel
{
	HWND _hWndParent;
	int _x0, _y0, _width, _height;
	int _in_height, _out_height;
	
	Item* _in_items;
	Item* _out_items;

	DrawFunc _DrawFunc;
	PosChangedFunc _PosChangedFunc;
	FontChangedFunc _FontChangedFunc;
	CalcSizeFunc _CalcSizeFunc;
	
	OnKey_LeftArrowFunc _OnKey_LeftArrowFunc;
	OnKey_RightArrowFunc _OnKey_RightArrowFunc;
	OnChar_DefaultFunc _OnChar_DefaultFunc;
	OnChar_BackspaceFunc _OnChar_BackspaceFunc;
	OnChar_ReturnFunc _OnChar_ReturnFunc;
} Panel;

Panel* Panel_init(HWND hWnd);
void Panel_free(Panel* p);

#endif /* _PANEL_H_ */

