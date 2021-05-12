#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"
#include "items.h"
#include "editor.h"

#define WM_PANEL_REPAINT WM_USER + 1
#define WM_PANEL_SIZE_CHANGED WM_USER + 2

typedef struct _Panel Panel;

typedef RECT(*GetRectFunc)(Panel* p);

typedef void (*OnPanelInitFunc)(Panel* p);
typedef void (*DrawFunc)(Panel* p, HDC hdc);
typedef void (*PosChangedFunc)(Panel* p);
typedef void (*FontChangedFunc)(Panel* p);

typedef void (*OnPanelSetFocusFunc)(Panel* p);
typedef void (*OnPanelKillFocusFunc)(Panel* p);

typedef void (*OnKey_PanelLeftArrowFunc)(Panel* p);
typedef void (*OnKey_PanelRightArrowFunc)(Panel* p);
typedef void (*OnChar_PanelDefaultFunc)(Panel* p, wchar_t ch);
typedef void (*OnChar_PanelBackspaceFunc)(Panel* p);
typedef void (*OnChar_PanelReturnFunc)(Panel* p);

typedef void (*OnPanel_CmdFunc)(Panel* p, int cmd);

typedef struct _Panel
{
	HWND _hWndParent;
	int _x0, _y0, _width, _height;

	int _in_baseLine, _in_width, _in_height;
	int _out_baseLine, _out_width, _out_height;
	
	Item* _in_items;
	Item* _out_items;

	Editor* _editor;

	GetRectFunc _GetRectFunc;
	
	OnPanelInitFunc _OnPanelInitFunc;
	DrawFunc _DrawFunc;
	PosChangedFunc _PosChangedFunc;
	FontChangedFunc _FontChangedFunc;

	OnPanelSetFocusFunc _OnSetFocusFunc;
	OnPanelKillFocusFunc _OnKillFocusFunc;
	
	OnKey_PanelLeftArrowFunc _OnKey_LeftArrowFunc;
	OnKey_PanelRightArrowFunc _OnKey_RightArrowFunc;
	OnChar_PanelDefaultFunc _OnChar_DefaultFunc;
	OnChar_PanelBackspaceFunc _OnChar_BackspaceFunc;
	OnChar_PanelReturnFunc _OnChar_ReturnFunc;

	OnPanel_CmdFunc _OnCmdFunc;
} Panel;

Panel* Panel_init(HWND hWnd);
void Panel_free(Panel* p);

#endif /* _PANEL_H_ */

