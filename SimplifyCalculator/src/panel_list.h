#ifndef _PANEL_LIST_H_
#define _PANEL_LIST_H_

#include "panel.h"

typedef struct _PanelLinkedList PanelLinkedList;

typedef void (*OnPListInitFunc)(PanelLinkedList* pll, HWND hWnd);
typedef Panel* (*AddNewPanelFunc)(PanelLinkedList* pll);
typedef void (*PListPosChangedFunc)(PanelLinkedList* pll);
typedef void (*OnPListFontChangedFunc)(PanelLinkedList* pll);
typedef int (*GetViewportWidthFunc)(PanelLinkedList* pll);
typedef int (*GetViewportHeightFunc)(PanelLinkedList* pll);
typedef void (*OnPListPaintFunc)(PanelLinkedList* pll, HDC hdc, RECT* rcPaint);

typedef struct _PanelNode
{
	Panel* _panel;

	struct _PanelNode* _next;
	struct _PanelNode* _prev;
} PanelNode;

typedef struct _PanelLinkedList
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
} PanelLinkedList;

PanelLinkedList* PanelLinkedList_init();
void PanelLinkedList_free(PanelLinkedList* pll);
void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p);

#endif /* _PANEL_LIST_H_ */


