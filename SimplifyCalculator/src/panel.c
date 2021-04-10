#include "platform.h"

#include "calcimpl.h"
#include "parser.h"
#include "panel.h"

static void SetCmdProperties(Panel* p);
static void CalcHeight(Panel* p);
static void OnPropertyChanged(Panel* p);
static void Draw(Panel* p, HDC hdc);
static void UpdateCaretPos(Panel* p);

static void OnKey_LeftArrow(Panel* p);
static void OnKey_RightArrow(Panel* p);

static void OnChar_Default(Panel* p, wchar_t ch);
static void OnChar_Backspace(Panel* p);
static void OnChar_Return(Panel* p);

extern HFONT g_bold_font, g_math_font, g_fixed_font;
extern TEXTMETRIC g_tmFixed;

static const int g_content_margin_h = 10, g_content_margin_v = 10;
static const int g_padding = 30;
static const int g_formula_x = 50;

Panel* Panel_init(HWND hWnd, const wchar_t* cstr1, const wchar_t* cstr2)
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_cnt_str_in = String_init();
	p->_cnt_str_out = String_init();
	p->_str_in = String_init();
	p->_str_out = String_init();

	p->_items_in = NULL;

	p->_SetCmdPropertiesFunc = SetCmdProperties;
	p->_CalcHeightFunc = CalcHeight;
	p->_OnPropertyChangedFunc = OnPropertyChanged;
	p->_DrawFunc = Draw;
	p->_UpdateCaretPosFunc = UpdateCaretPos;

	p->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	p->_OnKey_RightArrowFunc = OnKey_RightArrow;
	p->_OnChar_DefaultFunc = OnChar_Default;
	p->_OnChar_BackspaceFunc = OnChar_Backspace;
	p->_OnChar_ReturnFunc = OnChar_Return;

	p->_caret_idx = 0;
	p->_items_in = NULL;

	p->_hWndParent = hWnd;
	String_cpy(p->_cnt_str_in, cstr1);
	String_cpy(p->_cnt_str_out, cstr2);

	return p;
}

void Panel_free(Panel* p)
{
	if (p->_items_in)
	{
		ItemTree_free(&p->_items_in);
		p->_items_in = NULL;
	}

	String_free(p->_cnt_str_in);
	String_free(p->_cnt_str_out);
	String_free(p->_str_in);
	String_free(p->_str_out);

	free(p);
}

static void SetCmdProperties(Panel* p)
{
	{
		// calc _cmd_pos_x
		SIZE s;

		HDC hdc = GetDC(p->_hWndParent);
		SelectObject(hdc, g_bold_font);
		GetTextExtentPoint32(hdc, p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len, &s);
		p->_cmd_pos_x = s.cx;
		ReleaseDC(p->_hWndParent, hdc);
	}

	// Set cmd Properties
	p->_cmd_width = p->_width - g_content_margin_h * 2 - p->_cmd_pos_x - g_padding;
	p->_cmd_column = p->_cmd_width / g_tmFixed.tmAveCharWidth;
	p->_cmd_column = p->_cmd_column ? p->_cmd_column : 1;
	p->_cmd_row_count = (int)(p->_str_in->_len / p->_cmd_column) + (p->_str_in->_len % p->_cmd_column > 0 ? 1 : 0);
	p->_cmd_row_count = (p->_cmd_row_count <= 0) ? 1 : p->_cmd_row_count;
}

static void CalcHeight(Panel* p)
{
	p->_height1 = p->_cmd_row_count * g_tmFixed.tmHeight + g_content_margin_v;

	// calc _items_in height
	p->_height2 = 0;
	if(p->_items_in)
	{
		Graphics gfx;
		FontHandle fh; 
		HDC hdc = GetDC(p->_hWndParent);
		SelectObject(hdc, g_math_font);
		gfx._hdc = hdc;
		fh._hfont = g_math_font;
		p->_height2 = p->_items_in->_heightFunc(p->_items_in, &gfx) + g_content_margin_v;
		ReleaseDC(p->_hWndParent, hdc);
	}

	p->_height3 = 0;
	if (p->_str_out->_len > 0)
	{
		p->_height3 = g_tmFixed.tmHeight + g_content_margin_v;
	}

	p->_height = p->_height1 + p->_height2 + p->_height3 + g_content_margin_v;
}

static void OnPropertyChanged(Panel* p)
{
	if (p->_items_in)
	{
		Graphics gfx;
		FontHandle fh;
		HDC hdc = GetDC(p->_hWndParent);
		SelectObject(hdc, g_math_font);
		gfx._hdc = hdc;
		fh._hfont = g_math_font;

		int baseLine = p->_y + p->_height1 + g_content_margin_v + p->_items_in->_baseLineFunc(p->_items_in, &gfx);
		p->_items_in->_setFontFunc(p->_items_in, fh);
		p->_items_in->_calcCoordinateFunc(p->_items_in, &gfx, p->_x + g_formula_x, baseLine);
		ReleaseDC(p->_hWndParent, hdc);
	}
}

static void Draw(Panel* p, HDC hdc)
{
	RECT rc;
	rc.left = p->_x;
	rc.top = p->_y;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));

	// Draw cnt_in_str "In:"
	HGDIOBJ hOldFont = SelectObject(hdc, g_bold_font);
	TextOut(hdc, p->_x + g_content_margin_h, p->_y + g_content_margin_v,
		p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len);

	{
		SelectObject(hdc, g_fixed_font);
		int iy = 0;
		int ix = 0;
		while (ix < p->_str_in->_len)
		{
			int v = ix % p->_cmd_column;
			TextOut(hdc, p->_x + g_content_margin_h + p->_cmd_pos_x + g_padding + v * g_tmFixed.tmAveCharWidth,
				p->_y + g_content_margin_v + iy * g_tmFixed.tmHeight, p->_str_in->_str + ix, 1);
			if (v == p->_cmd_column - 1)
				++iy;
			++ix;
		}
	}

	if(p->_items_in)
	{
		SelectObject(hdc, g_math_font);
		Graphics gfx;
		gfx._hdc = hdc;
		p->_items_in->_drawFunc(p->_items_in, &gfx);
	}

	if (p->_str_out->_len > 0)
	{
		SIZE s;

		SelectObject(hdc, g_bold_font);
		TextOut(hdc, p->_x + g_content_margin_h, p->_y + g_content_margin_v + p->_height1 + p->_height2,
			p->_cnt_str_out->_str, (int)p->_cnt_str_out->_len);
		GetTextExtentPoint32(hdc, p->_cnt_str_out->_str, (int)p->_cnt_str_out->_len, &s);

		SelectObject(hdc, g_fixed_font); 
		TextOut(hdc, p->_x + g_content_margin_h + s.cx + g_padding, p->_y + g_content_margin_v + p->_height1 + p->_height2,
			p->_str_out->_str, (int)p->_str_out->_len);
	}

	SelectObject(hdc, hOldFont);
}

static void UpdateCaretPos(Panel* p)
{
	int caret_pos_x = 0, caret_pos_y = 0;
	
	int v1 = p->_caret_idx % p->_cmd_column;
	int v2 = p->_caret_idx / p->_cmd_column;
	caret_pos_x = p->_x + g_content_margin_h + p->_cmd_pos_x + g_padding + v1 * g_tmFixed.tmAveCharWidth;
	caret_pos_y = p->_y + g_content_margin_v + v2 * g_tmFixed.tmHeight;

	SetCaretPos(caret_pos_x, caret_pos_y);
}

static void OnKey_LeftArrow(Panel* p)
{
	if(p->_caret_idx > 0)
		--(p->_caret_idx);

	p->_UpdateCaretPosFunc(p);
}

static void OnKey_RightArrow(Panel* p)
{
	if(p->_caret_idx < p->_str_in->_len)
		++(p->_caret_idx);

	p->_UpdateCaretPosFunc(p);
}

static void OnChar_Default(Panel* p, wchar_t ch)
{
	static int x = 2;
	if (ch == L'^')
	{
		--x;
		if (x) return;
		x = 2;
	}

	String_insert_c(p->_str_in, p->_caret_idx, ch);

	++p->_caret_idx;
	p->_UpdateCaretPosFunc(p);

	PostMessage(p->_hWndParent, WM_PANEL_PROPERTY, (WPARAM)NULL, (LPARAM)p);
}

static void OnChar_Backspace(Panel* p)
{
	if (p->_caret_idx > 0)
	{
		--p->_caret_idx;
		String_delete_c(p->_str_in, p->_caret_idx);

		p->_UpdateCaretPosFunc(p);

		PostMessage(p->_hWndParent, WM_PANEL_PROPERTY, (WPARAM)NULL, (LPARAM)p);
	}
}

static void OnChar_Return(Panel* p)
{
	int rs;
	Item* items = NULL;

	if (p->_items_in)
	{
		ItemTree_free(&p->_items_in);
	}

	rs = parse(&items, p->_str_in->_str);
	if (!rs)
	{
		double result;
		wchar_t str[255];

		p->_items_in = items;

		rs = calculate((Item**)&p->_items_in, &result);
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
			String_cpy(p->_str_out, str);
		}
		else
		{
			swprintf_s(str, 255, L"Invalid value");
			String_cpy(p->_str_out, str);
		}
	}
	else
	{
		if (items)
		{
			ItemTree_free(&items);
		}

		p->_items_in = (Item*)ItemLiteral_init(L"Parse error");
	}

	PostMessage(p->_hWndParent, WM_PANEL_PROPERTY, (WPARAM)NULL, (LPARAM)p);
}
