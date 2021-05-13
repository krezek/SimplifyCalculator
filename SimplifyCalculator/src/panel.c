#include "platform.h"

#include "calcimpl.h"
#include "parser.h"
#include "panel.h"

static RECT GetRect(Panel* p);
static void OnInit(Panel* p);
static void OnPaint(Panel* p, HDC hdc);
static void OnPosChanged(Panel* p);
static void OnFontChanged(Panel* p);
static void OnSizeChanged(Panel* p);
static void SetItemsSize(Panel* p);
static void SetItemsCoordinate(Panel* p);
static void CalcPanelSize(Panel* p);

static void OnSetFocus(Panel* p);
static void OnKillFocus(Panel* p);

static void OnKey_LeftArrow(Panel* p);
static void OnKey_RightArrow(Panel* p);

static void OnChar_Default(Panel* p, wchar_t ch);
static void OnChar_Backspace(Panel* p);
static void OnChar_Return(Panel* p);

static void OnCmd(Panel* p, int cmd);

static const int g_margin_h = 10, g_margin_v = 10;

extern const wchar_t* g_in_str;
extern const wchar_t* g_out_str; 
extern int g_in_str_width, g_in_str_height;
extern int g_out_str_width, g_out_str_height;
extern int g_padding;

extern HFONT g_math_font;

Panel* Panel_init(HWND hWnd)
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_in_items = NULL;
	p->_out_items = NULL;

	p->_GetRectFunc = GetRect;

	p->_OnInitFunc = OnInit;
	p->_OnPaintFunc = OnPaint;
	p->_OnPosChangedFunc = OnPosChanged;
	p->_OnFontChangedFunc = OnFontChanged;
	p->_OnSizeChangedFunc = OnSizeChanged;
	
	p->_OnSetFocusFunc = OnSetFocus;
	p->_OnKillFocusFunc = OnKillFocus;
	
	p->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	p->_OnKey_RightArrowFunc = OnKey_RightArrow;
	p->_OnChar_DefaultFunc = OnChar_Default;
	p->_OnChar_BackspaceFunc = OnChar_Backspace;
	p->_OnChar_ReturnFunc = OnChar_Return;

	p->_OnCmdFunc = OnCmd;

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

static RECT GetRect(Panel* p)
{
	RECT rc;

	rc.left = p->_x0;
	rc.top = p->_y0;

	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	return rc;
}

static void OnInit(Panel* p)
{
	p->_editor->_hWnd = p->_hWndParent;
	p->_editor->_OnInitFunc(p->_editor);

	SetItemsSize(p);
	SetItemsCoordinate(p);
	CalcPanelSize(p);
}

static void OnPaint(Panel* p, HDC hdc)
{
	RECT rc;
	rc.left = p->_x0;
	rc.top = p->_y0;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));

	HFONT hOldFont = SelectObject(hdc,  g_math_font);

	{
		TextOut(hdc, p->_x0 + g_margin_v, p->_y0 + g_margin_h + 
			max(p->_in_baseLine, g_in_str_height / 2) - g_in_str_height / 2, g_in_str, (int)wcslen(g_in_str));
		TextOut(hdc, p->_x0 + g_margin_v, p->_y0 + 2 * g_margin_h + max(p->_in_height, g_in_str_height) + 
			max(p->_out_baseLine, g_out_str_height / 2) - g_out_str_height / 2, g_out_str, (int)wcslen(g_out_str));
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

static void OnPosChanged(Panel* p)
{
	SetItemsCoordinate(p);
}

static void OnFontChanged(Panel* p)
{
	SetItemsSize(p);
	SetItemsCoordinate(p);
	CalcPanelSize(p);
}

static void OnSizeChanged(Panel* p)
{
	SetItemsSize(p);
	SetItemsCoordinate(p);
	CalcPanelSize(p);
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

static void SetItemsCoordinate(Panel* p)
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
			max(p->_in_baseLine, g_in_str_height / 2) + p->_y0 + g_margin_h);
	}
	
	if (p->_out_items)
	{
		p->_out_baseLine = p->_out_items->_baseLineFunc(p->_out_items, &gfx);

		p->_out_items->_setFontFunc(p->_out_items, fh);
		p->_out_items->_calcCoordinateFunc(p->_out_items, &gfx, 
			x, 
			max(p->_out_baseLine, g_out_str_height / 2) + p->_y0 + max(p->_in_height, g_in_str_height) + 2 * g_margin_h);
	}

	ReleaseDC(p->_hWndParent, hdc);
}

static void CalcPanelSize(Panel* p)
{
	p->_width = 2 * g_margin_v + max(g_in_str_width, g_out_str_width) + g_padding + max(p->_in_width, p->_out_width);
	p->_height = max(g_in_str_height, p->_in_height) + max(g_out_str_height, p->_out_height) + 3 * g_margin_h;
}

static void OnSetFocus(Panel* p)
{
	if (p->_editor)
		p->_editor->_OnSetFocusFunc(p->_editor);
}

static void OnKillFocus(Panel* p)
{
	if (p->_editor)
		p->_editor->_OnKillFocusFunc(p->_editor);
}

static void OnKey_LeftArrow(Panel* p)
{
	if (p->_editor)
		p->_editor->_OnKey_LeftArrowFunc(p->_editor);
}

static void OnKey_RightArrow(Panel* p)
{
	if (p->_editor)
		p->_editor->_OnKey_RightArrowFunc(p->_editor);
}

static void OnChar_Default(Panel* p, wchar_t ch)
{
	if (p->_editor)
		p->_editor->_OnChar_DefaultFunc(p->_editor, ch);
}

static void OnChar_Backspace(Panel* p)
{
}

static void OnChar_Return(Panel* p)
{
	int rs;

	if (p->_out_items)
	{
		ItemTree_free(&p->_out_items);
	}

	double result;
	wchar_t str[255];

	rs = calculate((Item**)&p->_in_items, &result);
	if (rs == 0)
	{
		long long lrs = (long long)result;

		if (result == (double)lrs)
		{
			swprintf_s(str, 255, L"%lld", lrs);
		}
		else
		{
			swprintf_s(str, 255, L"%e", result);
		}
	}
	else
	{
		swprintf_s(str, 255, L"Invalid value");
	}

	p->_out_items = (Item*)ItemLiteral_init(str);
}

static void OnCmd(Panel* p, int cmd)
{
	p->_editor->_OnCmdFunc(p->_editor, cmd);
}
