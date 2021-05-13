#ifndef _PANEL_LIST_H_
#define _PANEL_LIST_H_

#include "panel.h"

typedef struct _PanelList PanelList;

typedef void (*OnPListInitFunc)(PanelList* pll, HWND hWnd);
typedef Panel* (*AddNewPanelFunc)(PanelList* pll);
typedef void (*OnPListPosChangedFunc)(PanelList* pll);
typedef void (*OnPListFontChangedFunc)(PanelList* pll);
typedef void (*OnPListSelectedPanelChangedFunc)(PanelList* pll);
typedef int (*GetViewportWidthFunc)(PanelList* pll);
typedef int (*GetViewportHeightFunc)(PanelList* pll);
typedef void (*OnPListPaintFunc)(PanelList* pll, HDC hdc, RECT* rcPaint);
typedef Panel* (*GetPanelFromPointFunc)(PanelList* pll, int px, int py);

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
	OnPListPosChangedFunc _OnPosChangedFunc;
	OnPListFontChangedFunc _OnFontChangedFunc;
	OnPListSelectedPanelChangedFunc _OnSelectedPanelChanged;
	GetViewportWidthFunc _GetViewportWidthFunc;
	GetViewportHeightFunc _GetViewportHeightFunc;
	OnPListPaintFunc _OnPaintFunc;
	GetPanelFromPointFunc _GetPanelFromPointFunc;
} PanelList;

PanelList* PanelList_init();
void PanelList_free(PanelList* pll);
void PanelList_pushpack(PanelList* pll, Panel* p);

#endif /* _PANEL_LIST_H_ */


