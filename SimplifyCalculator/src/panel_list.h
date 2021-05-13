#ifndef _PANEL_LIST_H_
#define _PANEL_LIST_H_

#include "panel.h"

typedef struct _PanelList PanelList;

typedef void (*OnPListInitFunc)(PanelList* pll, HWND hWnd);
typedef Panel* (*AddNewPanelFunc)(PanelList* pll);
typedef void (*PListPosChangedFunc)(PanelList* pll);
typedef void (*OnPListFontChangedFunc)(PanelList* pll);
typedef int (*GetViewportWidthFunc)(PanelList* pll);
typedef int (*GetViewportHeightFunc)(PanelList* pll);
typedef void (*OnPListPaintFunc)(PanelList* pll, HDC hdc, RECT* rcPaint);

typedef struct _PanelNode
{
	Panel* _panel;

	struct _PanelNode* _next;
	struct _PanelNode* _prev;
} PanelNode;

typedef struct _PanelList
{
	PanelNode* _front;
	PanelNode* _rear;

	HWND _hWndParent;
	int _x0, _y0, _width, _height;
	int _x_current_pos, _y_current_pos;

	Panel* _selected_panel;

	OnPListInitFunc _OnInitFunc;
	AddNewPanelFunc _AddNewPanelFunc;
	PListPosChangedFunc _PosChangedFunc;
	OnPListFontChangedFunc _OnFontChangedFunc;
	GetViewportWidthFunc _GetViewportWidthFunc;
	GetViewportHeightFunc _GetViewportHeightFunc;
	OnPListPaintFunc _OnPaintFunc;
} PanelList;

PanelList* PanelList_init();
void PanelList_free(PanelList* pll);
void PanelList_pushpack(PanelList* pll, Panel* p);

#endif /* _PANEL_LIST_H_ */


