#ifndef _PANEL_LIST_H_
#define _PANEL_LIST_H_

#include "panel.h"

typedef struct _PanelLinkedList PanelLinkedList;

typedef void (*OnInitializeFunc)(PanelLinkedList* pll);
typedef void (*ParentPropertyChangedFunc)(PanelLinkedList* pll);
typedef void (*ParentPosChangedFunc)(PanelLinkedList* pll);
typedef int (*GetViewportHeightFunc)(PanelLinkedList* pll);
typedef void (*DrawListFunc)(PanelLinkedList* pll, HDC hdc, RECT* rcPaint);
typedef Panel* (*AddNewTicketFunc)(PanelLinkedList* pll);

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
	int _client_width, _client_height;
	int _x_current_pos, _y_current_pos;

	Panel* _selected_panel;

	OnInitializeFunc _OnInitializeFunc;
	ParentPropertyChangedFunc _ParentPropertyChangedFunc;
	ParentPosChangedFunc _ParentPosChangedFunc;
	GetViewportHeightFunc _GetViewportHeightFunc;
	DrawListFunc _DrawListFunc;
	AddNewTicketFunc _AddNewTicketFunc;
} PanelLinkedList;

PanelLinkedList* PanelLinkedList_init();
void PanelLinkedList_free(PanelLinkedList* pll);
void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p);

#endif /* _PANEL_LIST_H_ */


