#ifndef _PANEL_H_
#define _PANEL_H_

#include "strg.h"
#include "items.h"

#define WM_PANEL_PROPERTY WM_USER+1

typedef struct _Panel Panel;

typedef void (*SetCmdPropertiesFunc)(Panel* p); 
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

	int _cmd_pos_x, _cmd_width;
	int _cmd_column, _cmd_row_count;

	int _caret_idx;

	SetCmdPropertiesFunc _SetCmdPropertiesFunc;
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

Panel* Panel_init(HWND hWnd, const wchar_t* cstr1, const wchar_t* cstr2);
void Panel_free(Panel* p);

#endif /* _PANEL_H_ */

