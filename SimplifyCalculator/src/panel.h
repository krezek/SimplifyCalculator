#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"
#include "items.h"

#define WM_PANEL_PROPERTY WM_USER+1

typedef struct _Panel Panel;
typedef struct _PanelLinkedList PanelLinkedList;

typedef void (*OnInitializeFunc)(PanelLinkedList* pll);
typedef void (*ParentPropertyChangedFunc)(PanelLinkedList* pll);
typedef void (*ParentPosChangedFunc)(PanelLinkedList* pll);
typedef int (*GetViewportHeightFunc)(PanelLinkedList* pll);
typedef void (*DrawListFunc)(PanelLinkedList * pll, HDC hdc, RECT* rcPaint);
typedef Panel* (*AddNewTicketFunc)(PanelLinkedList* pll);

typedef void (*CalcHeightFunc)(Panel* p);
typedef void (*OnPropertyChangedFunc)(Panel* p);
typedef void (*DrawFunc)(Panel* p, HDC hdc);
typedef void (*UpdateCaretPosFunc)(Panel* p);

typedef void (*OnKey_LeftArrowFunc)(Panel* p);
typedef void (*OnKey_RightArrowFunc)(Panel* p);
typedef void (*OnChar_DefaultFunc)(Panel* p, wchar_t ch);
typedef void (*OnChar_BackspaceFunc)(Panel* p);
typedef void (*OnChar_ReturnFunc)(Panel* p);

typedef struct _Panel
{
	HWND _hWndParent;
	int _x, _y, _width, _height;
	int _height1, _height2, _height3;
	
	String* _cnt_str_in;
	String* _cnt_str_out;
	String* _str_in;
	String* _str_out;

	Item* _items_in;

	int _cmd_pos_x;
	int _caret_idx;

	CalcHeightFunc _CalcHeightFunc;
	OnPropertyChangedFunc _OnPropertyChangedFunc;
	DrawFunc _DrawFunc;
	UpdateCaretPosFunc _UpdateCaretPosFunc;

	OnKey_LeftArrowFunc _OnKey_LeftArrowFunc;
	OnKey_RightArrowFunc _OnKey_RightArrowFunc;
	OnChar_DefaultFunc _OnChar_DefaultFunc;
	OnChar_BackspaceFunc _OnChar_BackspaceFunc;
	OnChar_ReturnFunc _OnChar_ReturnFunc;
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

	Panel* _selected_panel;

	OnInitializeFunc _OnInitializeFunc;
	ParentPropertyChangedFunc _ParentPropertyChangedFunc;
	ParentPosChangedFunc _ParentPosChangedFunc;
	GetViewportHeightFunc _GetViewportHeightFunc;
	DrawListFunc _DrawListFunc;
	AddNewTicketFunc _AddNewTicketFunc;
} PanelLinkedList;

Panel* Panel_init();
void Panel_free(Panel* p);

PanelNode* PanelNode_init(Panel* p, PanelNode* nxt, PanelNode* prv);
void PanelNode_free(PanelNode* pn);

PanelLinkedList* PanelLinkedList_init();
void PanelLinkedList_free(PanelLinkedList* pll);
void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p);

#endif /* _PANEL_H_ */

