#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"

typedef struct _Panel Panel;
typedef struct _PanelLinkedList PanelLinkedList;

typedef void (*OnInitializeFunc)(PanelLinkedList* pll);
typedef void (*ParentSizeChangedFunc)(PanelLinkedList* pll);
typedef int (*GetViewportHeightFunc)(PanelLinkedList* pll);
typedef void (*DrawListFunc)(PanelLinkedList * pll, HDC hdc);
typedef void (*CalcHeightFunc)(Panel* p);
typedef void (*DrawFunc)(Panel* p, HDC hdc);
typedef void (*ShowCaretFunc)(Panel* p);

typedef struct _Panel
{
	HWND _hWndParent;
	int _x, _y, _width, _height;
	
	String* _cnt_str_in;
	String* _cnt_str_out;
	String* _str_in;
	String* _str_out;

	int _cmd_pos_x;
	int _caret_pos_x, _caret_pos_y;

	CalcHeightFunc _CalcHeightFunc;
	DrawFunc _DrawFunc;
	ShowCaretFunc _ShowCaretFunc;
} Panel;

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

	Panel* _current_panel;

	OnInitializeFunc _OnInitializeFunc;
	ParentSizeChangedFunc _ParentSizeChangedFunc;
	GetViewportHeightFunc _GetViewportHeightFunc;
	DrawListFunc _DrawListFunc;
} PanelLinkedList;

Panel* Panel_init();
void Panel_free(Panel* p);

PanelNode* PanelNode_init(Panel* p, PanelNode* nxt, PanelNode* prv);
void PanelNode_free(PanelNode* pn);

PanelLinkedList* PanelLinkedList_init();
void PanelLinkedList_free(PanelLinkedList* pll);
void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p);

#endif /* _PANEL_H_ */

