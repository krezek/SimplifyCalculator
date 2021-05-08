#include "platform.h"

#include "calcimpl.h"
#include "parser.h"
#include "panel.h"

static void Draw(Panel* p, HDC hdc);
static void PosChanged(Panel* p);
static void CalcSize(Panel* p);

static void OnKey_LeftArrow(Panel* p);
static void OnKey_RightArrow(Panel* p);

static void OnChar_Default(Panel* p, wchar_t ch);
static void OnChar_Backspace(Panel* p);
static void OnChar_Return(Panel* p);

static const wchar_t* g_in_str = L"In:";
static const wchar_t* g_out_str = L"Out:";

static const int g_margin_h = 10, g_margin_v = 10;
static const int g_padding = 30;

Panel* Panel_init(HWND hWnd)
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_in_items = NULL;
	p->_out_items = NULL;

	p->_DrawFunc = Draw;
	p->_PosChangedFunc = PosChanged;
	p->_CalcSizeFunc = CalcSize;
	
	p->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	p->_OnKey_RightArrowFunc = OnKey_RightArrow;
	p->_OnChar_DefaultFunc = OnChar_Default;
	p->_OnChar_BackspaceFunc = OnChar_Backspace;
	p->_OnChar_ReturnFunc = OnChar_Return;

	p->_hWndParent = hWnd;

	return p;
}

void Panel_free(Panel* p)
{
	if (p->_in_items)
	{
		ItemTree_free(&p->_in_items);
		p->_in_items = NULL;
	}

	if (p->_out_items)
	{
		ItemTree_free(&p->_out_items);
		p->_out_items = NULL;
	}

	free(p);
}

static void Draw(Panel* p, HDC hdc)
{
	RECT rc;
	rc.left = p->_x0;
	rc.top = p->_y0;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));

	if(p->_in_items)
	{
		//SelectObject(hdc, g_math_font);
		Graphics gfx;
		gfx._hdc = hdc;
		p->_in_items->_drawFunc(p->_in_items, &gfx);
	}

	if (p->_out_items)
	{
		//SelectObject(hdc, g_math_font);
		Graphics gfx;
		gfx._hdc = hdc;
		p->_out_items->_drawFunc(p->_out_items, &gfx);
	}

	//SelectObject(hdc, hOldFont);
}

static void PosChanged(Panel* p)
{

}

static void CalcSize(Panel* p)
{
	int cnt_width = 0, var_width = 0;
	int var_height = 0;
	SIZE sz1, sz2;

	HDC hdc = GetDC(p->_hWndParent);

	GetTextExtentPoint(hdc, g_in_str, (int)wcslen(g_in_str), &sz1);
	GetTextExtentPoint(hdc, g_out_str, (int)wcslen(g_out_str), &sz2);
	cnt_width = max(sz1.cx, sz2.cx);

	if (p->_in_items)
	{
		var_width += max(var_width, p->_in_items->_width);
		var_height += max(sz1.cy, p->_in_items->_height);
	}

	if (p->_out_items)
	{
		var_width += max(var_width, p->_out_items->_width);
		var_height += max(sz2.cy, p->_out_items->_height);
	}

	var_height = max(var_height, max(sz1.cy, sz2.cy));

	if (p->_in_items && p->_out_items)
		var_height += g_margin_h;

	p->_width = cnt_width + var_width + g_padding + 2 * g_margin_v;
	p->_height = var_height + 2 * g_margin_h;

	ReleaseDC(p->_hWndParent, hdc);
}

/*static void Panel_Update(Panel* p)
{
	int row_count = (int)(p->_str_in->_len / p->_cmd_column) + (p->_str_in->_len % p->_cmd_column > 0 ? 1 : 0);
	row_count = (row_count <= 0) ? 1 : row_count;

	if (row_count == p->_cmd_row_count)
	{
		PostMessage(p->_hWndParent, WM_PANEL_REPAINT, (WPARAM)NULL, (LPARAM)p);
	}
	else
	{
		PostMessage(p->_hWndParent, WM_PANEL_SIZE_CHANGED, (WPARAM)NULL, (LPARAM)p);
	}
}*/

static void OnKey_LeftArrow(Panel* p)
{
}

static void OnKey_RightArrow(Panel* p)
{
}

static void OnChar_Default(Panel* p, wchar_t ch)
{
}

static void OnChar_Backspace(Panel* p)
{
}

static void OnChar_Return(Panel* p)
{
}
