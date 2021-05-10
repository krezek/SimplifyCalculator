#include "platform.h"

#include "calcimpl.h"
#include "parser.h"
#include "panel.h"

static void OnInit(Panel* p);
static void Draw(Panel* p, HDC hdc);
static void PosChanged(Panel* p);
static void FontChanged(Panel* p);
static void SetCntSize(Panel* p);
static void SetItemsSize(Panel* p);
static void SetCoordinate(Panel* p);
static void CalcPanelSize(Panel* p);

static void OnKey_LeftArrow(Panel* p);
static void OnKey_RightArrow(Panel* p);

static void OnChar_Default(Panel* p, wchar_t ch);
static void OnChar_Backspace(Panel* p);
static void OnChar_Return(Panel* p);

static const wchar_t* g_in_str = L"In:";
static const wchar_t* g_out_str = L"Out:";

static const int g_margin_h = 10, g_margin_v = 10;

static int g_in_str_width, g_in_str_height;
static int g_out_str_width, g_out_str_height;
static int g_padding;

extern HFONT g_math_font;

Panel* Panel_init(HWND hWnd)
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_in_items = NULL;
	p->_out_items = NULL;

	p->_OnPanelInitFunc = OnInit;
	p->_DrawFunc = Draw;
	p->_PosChangedFunc = PosChanged;
	p->_FontChangedFunc = FontChanged;
	
	p->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	p->_OnKey_RightArrowFunc = OnKey_RightArrow;
	p->_OnChar_DefaultFunc = OnChar_Default;
	p->_OnChar_BackspaceFunc = OnChar_Backspace;
	p->_OnChar_ReturnFunc = OnChar_Return;

	p->_hWndParent = hWnd;

	p->_editor = Editor_init(&p->_in_items);

	return p;
}

void Panel_free(Panel* p)
{
	if (p->_editor)
	{
		Editor_free(p->_editor);
	}

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

static void OnInit(Panel* p)
{
	p->_editor->_OnEditorInitializeFunc(p->_editor);

	SetCntSize(p);
	SetCoordinate(p);
	SetItemsSize(p);
	CalcPanelSize(p);
}

static void Draw(Panel* p, HDC hdc)
{
	RECT rc;
	rc.left = p->_x0;
	rc.top = p->_y0;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));

	HFONT hOldFont = SelectObject(hdc,  g_math_font);

	{
		TextOut(hdc, p->_x0 + g_margin_v, p->_y0 + g_margin_h + p->_in_baseLine - g_in_str_height / 2, g_in_str, (int)wcslen(g_in_str));
		TextOut(hdc, p->_x0 + g_margin_v, p->_y0 + 2 * g_margin_h + p->_in_height + p->_out_baseLine, g_out_str, (int)wcslen(g_out_str));
	}

	if(p->_in_items)
	{
		Graphics gfx;
		gfx._hdc = hdc;
		p->_in_items->_drawFunc(p->_in_items, &gfx);
	}

	if (p->_out_items)
	{
		Graphics gfx;
		gfx._hdc = hdc;
		p->_out_items->_drawFunc(p->_out_items, &gfx);
	}

	SelectObject(hdc, hOldFont);
}

static void PosChanged(Panel* p)
{
	SetCoordinate(p);
}

static void FontChanged(Panel* p)
{
	SetCntSize(p);
	SetCoordinate(p);
	SetItemsSize(p);
	CalcPanelSize(p);
}

static void SetCntSize(Panel* p)
{
	SIZE sz1, sz2, sz3;

	HDC hdc = GetDC(p->_hWndParent);
	SelectObject(hdc, g_math_font);

	GetTextExtentPoint(hdc, g_in_str, (int)wcslen(g_in_str), &sz1);
	GetTextExtentPoint(hdc, g_out_str, (int)wcslen(g_out_str), &sz2);
	GetTextExtentPoint(hdc, L"W", 1, &sz3);

	g_in_str_width = sz1.cx;
	g_in_str_height = sz1.cy;

	g_out_str_width = sz2.cx;
	g_out_str_height = sz2.cy;

	g_padding = sz3.cx;

	ReleaseDC(p->_hWndParent, hdc);
}

static void SetItemsSize(Panel* p)
{
	HDC hdc = GetDC(p->_hWndParent);
	SelectObject(hdc, g_math_font);

	Graphics gfx;
	gfx._hdc = hdc;
	
	p->_in_width = p->_in_height = 0;
	p->_out_width = p->_out_height = 0;

	if (p->_in_items)
	{
		p->_in_width = p->_in_items->_widthFunc(p->_in_items, &gfx);
		p->_in_height = p->_in_items->_heightFunc(p->_in_items, &gfx);
	}

	if (p->_out_items)
	{
		p->_out_width = p->_out_items->_widthFunc(p->_out_items, &gfx);
		p->_out_height = p->_out_items->_heightFunc(p->_out_items, &gfx);
	}

	ReleaseDC(p->_hWndParent, hdc);
}

static void SetCoordinate(Panel* p)
{
	HDC hdc = GetDC(p->_hWndParent);
	SelectObject(hdc, g_math_font);

	Graphics gfx;
	gfx._hdc = hdc;

	FontHandle fh;
	fh._hfont = g_math_font;

	p->_in_baseLine = p->_out_baseLine = 0;
	int x = p->_x0 + g_margin_v + max(g_in_str_width, g_out_str_width) + g_padding;

	if (p->_in_items)
	{
		p->_in_baseLine = p->_in_items->_baseLineFunc(p->_in_items, &gfx);

		p->_in_items->_setFontFunc(p->_in_items, fh);
		p->_in_items->_calcCoordinateFunc(p->_in_items, &gfx, 
			x, 
			p->_in_baseLine + p->_y0 + g_margin_h);
	}
	
	if (p->_out_items)
	{
		p->_out_baseLine = p->_out_items->_baseLineFunc(p->_out_items, &gfx);

		p->_out_items->_setFontFunc(p->_out_items, fh);
		p->_out_items->_calcCoordinateFunc(p->_out_items, &gfx, 
			x, 
			p->_out_baseLine + p->_y0 + g_margin_h);
	}

	ReleaseDC(p->_hWndParent, hdc);
}

static void CalcPanelSize(Panel* p)
{
	p->_width = 2 * g_margin_v + max(g_in_str_width, g_out_str_width) + g_padding + max(p->_in_width, p->_out_width);
	p->_height = max(g_in_str_height, p->_in_height) + max(g_out_str_height, p->_out_height) + 3 * g_margin_h;
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
