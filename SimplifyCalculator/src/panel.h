#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"
#include "items.h"
#include "editor.h"

typedef struct _Panel Panel;

typedef RECT(*GetRectFunc)(Panel* p);

typedef void (*OnPanelInitFunc)(Panel* p);
typedef void (*OnPanelPaint)(Panel* p, HDC hdc);
typedef void (*OnPanelPosChangedFunc)(Panel* p);
typedef void (*OnPanelFontChangedFunc)(Panel* p);
typedef void (*OnPanelSizeChangedFunc)(Panel* p);

typedef void (*OnPanelSetFocusFunc)(Panel* p);
typedef void (*OnPanelKillFocusFunc)(Panel* p);

typedef void (*OnKey_PanelLeftArrowFunc)(Panel* p);
typedef void (*OnKey_PanelRightArrowFunc)(Panel* p);
typedef void (*OnChar_PanelDefaultFunc)(Panel* p, wchar_t ch);
typedef void (*OnChar_PanelBackspaceFunc)(Panel* p);
typedef void (*OnChar_PanelReturnFunc)(Panel* p);
typedef void (*OnChar_PanelDeleteFunc)(Panel* p);

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
	
	OnPanelInitFunc _OnInitFunc;
	OnPanelPaint _OnPaintFunc;
	OnPanelPosChangedFunc _OnPosChangedFunc;
	OnPanelFontChangedFunc _OnFontChangedFunc;
	OnPanelSizeChangedFunc _OnSizeChangedFunc;

	OnPanelSetFocusFunc _OnSetFocusFunc;
	OnPanelKillFocusFunc _OnKillFocusFunc;
	
	OnKey_PanelLeftArrowFunc _OnKey_LeftArrowFunc;
	OnKey_PanelRightArrowFunc _OnKey_RightArrowFunc;
	OnChar_PanelDefaultFunc _OnChar_DefaultFunc;
	OnChar_PanelBackspaceFunc _OnChar_BackspaceFunc;
	OnChar_PanelReturnFunc _OnChar_ReturnFunc;
	OnChar_PanelDeleteFunc _OnChar_DeleteFunc;

	OnPanel_CmdFunc _OnCmdFunc;
} Panel;

Panel* Panel_init(HWND hWnd);
void Panel_free(Panel* p);

#endif /* _PANEL_H_ */

