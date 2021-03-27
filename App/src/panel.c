#include "platform.h"

#include "parser.h"
#include "panel.h"

extern HFONT g_bold_font, g_math_font, g_fixed_font;
extern TEXTMETRIC g_tmFixed;

static const int g_panel_margin_h = 10, g_panel_margin_v = 10;
static const int g_content_margin_h = 10, g_content_margin_v = 10;
static const int g_padding = 30;
static const int g_formula_x = 50;

static void OnInitialze(PanelLinkedList* pll);
static void ParentPropertyChanged(PanelLinkedList* pll);
static void ParentPosChanged(PanelLinkedList* pll);
static int GetViewportHeight(PanelLinkedList* pll);
static void DrawList(PanelLinkedList* pll, HDC hdc);

static void CalcHeight(Panel* p);
static void OnPropertyChanged(Panel* p);
static void Draw(Panel* p, HDC hdc);
static void UpdateCaretPos(Panel* p);

static void OnLeftArrow(Panel* p);
static void OnRightArrow(Panel* p);

static void OnChar(Panel* p, wchar_t ch);

PanelNode* PanelNode_init(Panel* p, PanelNode* nxt, PanelNode* prv)
{
	PanelNode* pn = (PanelNode*)malloc(sizeof(PanelNode));
	assert(pn);

	pn->_panel= p;
	pn->_next = nxt;
	pn->_prev = prv;

	return pn;
}

void PanelNode_free(PanelNode* pn)
{
	if (pn->_panel)
	{
		Panel_free(pn->_panel);
	}

	free(pn);
}

PanelLinkedList* PanelLinkedList_init()
{
	PanelLinkedList* pll = (PanelLinkedList*)malloc(sizeof(PanelLinkedList));
	assert(pll != NULL);

	pll->_front = NULL;
	pll->_rear = NULL;

	pll->_client_width = pll->_client_height = 0;
	pll->_x_current_pos = pll->_y_current_pos = 0;

	pll->_OnInitializeFunc = OnInitialze;
	pll->_ParentPropertyChangedFunc = ParentPropertyChanged;
	pll->_ParentPosChangedFunc = ParentPosChanged;
	pll->_GetViewportHeightFunc = GetViewportHeight;
	pll->_DrawListFunc = DrawList;

	return pll;
}

void PanelLinkedList_free(PanelLinkedList* pll)
{
	if (pll)
	{
		if (pll->_front)
		{
			while (pll->_front)
			{
				PanelNode* pn = pll->_front;
				pll->_front = pll->_front->_next;

				PanelNode_free(pn);
			}

			pll->_front = NULL;
			pll->_rear = NULL;
		}

		free(pll);
	}
}

void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p)
{
	if (pll->_rear == NULL)
	{
		assert(pll->_front == NULL);
		pll->_front = pll->_rear = PanelNode_init(p, NULL, NULL);
	}
	else
	{
		PanelNode* pn = PanelNode_init(p, NULL, pll->_rear);
		pll->_rear->_next = pn;
		pll->_rear = pn;
	}
}

static void OnInitialze(PanelLinkedList* pll)
{
	Item* items = NULL;
	int rs;

	Panel* p1 = Panel_init();
	p1->_hWndParent = pll->_hWndParent;
	String_cpy(p1->_cnt_str_in, L"In:");
	String_cpy(p1->_str_in, L"x_1=(-b+sqrt(b^2-4*a*c))/(2*a),x_2=(-b-sqrt(b^2-4*a*c))/(2*a)");
	String_cpy(p1->_cnt_str_out, L"Value:");
	rs = parse(&items, p1->_str_in->_str);
	if (!rs)
	{
		p1->_items_in = items;
	}
	else
	{
		if (items)
		{
			ItemTree_free(&items);
		}

		p1->_items_in = (Item*)ItemLiteral_init(L"Parse error");
	}
	PanelLinkedList_pushpack(pll, p1);

	Panel* p2 = Panel_init();
	p2->_hWndParent = pll->_hWndParent;
	String_cpy(p2->_cnt_str_in, L"In:");
	String_cpy(p2->_str_in, L"n_0*x^n+n_1*x^(n-1)+n_2*x^(n-2)+tdot+n_(k-2)*x^2+n_(k-1)*x+n_k=0");
	String_cpy(p2->_cnt_str_out, L"Value:");
	rs = parse(&items, p2->_str_in->_str);
	if (!rs)
	{
		p2->_items_in = items;
	}
	else
	{
		if (items)
		{
			ItemTree_free(&items);
		}

		p2->_items_in = (Item*)ItemLiteral_init(L"Parse error");
	}
	PanelLinkedList_pushpack(pll, p2);

	Panel* p3 = Panel_init();
	p3->_hWndParent = pll->_hWndParent;
	String_cpy(p3->_cnt_str_in, L"In:");
	String_cpy(p3->_str_in, L"Cos(x;2)+Sin(x;2)=1");
	String_cpy(p3->_cnt_str_out, L"Value:");
	rs = parse(&items, p3->_str_in->_str);
	if (!rs)
	{
		p3->_items_in = items;
	}
	else
	{
		if (items)
		{
			ItemTree_free(&items);
		}

		p3->_items_in = (Item*)ItemLiteral_init(L"Parse error");
	}
	PanelLinkedList_pushpack(pll, p3);

	pll->_selected_panel = p1;
}

static void ParentPropertyChanged(PanelLinkedList* pll)
{
	int x = g_panel_margin_h;
	int y = g_panel_margin_v - pll->_y_current_pos;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_x = x;
				pn->_panel->_y = y;
				pn->_panel->_width = pll->_client_width - g_panel_margin_h * 2;
				pn->_panel->_CalcHeightFunc(pn->_panel);
				pn->_panel->_OnPropertyChangedFunc(pn->_panel);

				y += pn->_panel->_height + g_panel_margin_v;

				pn = pn->_next;
			}
		}
	}
}

static void ParentPosChanged(PanelLinkedList* pll)
{
	int x = g_panel_margin_h;
	int y = g_panel_margin_v - pll->_y_current_pos;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_x = x;
				pn->_panel->_y = y;

				pn->_panel->_OnPropertyChangedFunc(pn->_panel);
				
				y += pn->_panel->_height + g_panel_margin_v;

				pn = pn->_next;
			}
		}
	}
}

static int GetViewportHeight(PanelLinkedList* pll)
{
	int y = g_panel_margin_v;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				y += pn->_panel->_height + g_panel_margin_v;
				pn = pn->_next;
			}
		}
	}

	return y;
}

static void DrawList(PanelLinkedList* pll, HDC hdc)
{
	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_DrawFunc(pn->_panel, hdc);
				pn = pn->_next;
			}
		}
	}
}

Panel* Panel_init()
{
	Panel* p = (Panel*)malloc(sizeof(Panel));
	assert(p != NULL);

	p->_cnt_str_in = String_init();
	p->_cnt_str_out = String_init();
	p->_str_in = String_init();
	p->_str_out = String_init();

	p->_CalcHeightFunc = CalcHeight;
	p->_OnPropertyChangedFunc = OnPropertyChanged;
	p->_DrawFunc = Draw;
	p->_UpdateCaretPosFunc = UpdateCaretPos;

	p->_OnLeftArrowFunc = OnLeftArrow;
	p->_OnRightArrowFunc = OnRightArrow;
	p->_OnCharFunc = OnChar;

	p->_caret_idx = 0;
	p->_items_in = NULL;

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

static void CalcHeight(Panel* p)
{
	// calc _str_in height
	{
		int w1 = p->_width - g_content_margin_h * 2 - p->_cmd_pos_x - g_padding;
		int col1 = w1 / g_tmFixed.tmAveCharWidth;
		col1 = col1 ? col1 : 1;
		int row_count = (int)(p->_str_in->_len / col1) + (p->_str_in->_len % col1 > 0 ? 1 : 0);

		p->_height1 = row_count * g_tmFixed.tmHeight + 2 * g_content_margin_v;
	}

	// calc _items_in height
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

	p->_height = p->_height1 + p->_height2 + g_content_margin_v;
}

static void OnPropertyChanged(Panel* p)
{
	{
		// calc _cmd_pos_x
		SIZE s1;

		HDC hdc = GetDC(p->_hWndParent);
		SelectObject(hdc, g_bold_font);
		GetTextExtentPoint32(hdc, p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len, &s1);
		p->_cmd_pos_x = s1.cx;
		ReleaseDC(p->_hWndParent, hdc);
	}

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
		int w1 = p->_width - g_content_margin_h * 2 - p->_cmd_pos_x - g_padding;
		int col1 = w1 / g_tmFixed.tmAveCharWidth;
		col1 = col1 ? col1 : 1;

		int iy = 0;
		int ix = 0;
		while (ix < p->_str_in->_len)
		{
			int v = ix % col1;
			TextOut(hdc, p->_x + g_content_margin_h + p->_cmd_pos_x + g_padding + v * g_tmFixed.tmAveCharWidth,
				p->_y + g_content_margin_v + iy * g_tmFixed.tmHeight, p->_str_in->_str + ix, 1);
			if (v == col1 - 1)
				++iy;
			++ix;
		}
	}

	{
		SelectObject(hdc, g_math_font);
		Graphics gfx;
		gfx._hdc = hdc;
		p->_items_in->_drawFunc(p->_items_in, &gfx);
	}

	SelectObject(hdc, hOldFont);
}

static void UpdateCaretPos(Panel* p)
{
	int w1 = p->_width - g_content_margin_h * 2 - p->_cmd_pos_x - g_padding;
	int col1 = w1 / g_tmFixed.tmAveCharWidth;
	col1 = col1 ? col1 : 1;

	int caret_pos_x = 0, caret_pos_y = 0;
	
	col1++;
	int v1 = p->_caret_idx % col1;
	int v2 = p->_caret_idx / col1;
	caret_pos_x = p->_x + g_content_margin_h + p->_cmd_pos_x + g_padding + v1 * g_tmFixed.tmAveCharWidth;
	caret_pos_y = p->_y + g_content_margin_v + v2 * g_tmFixed.tmHeight;

	SetCaretPos(caret_pos_x, caret_pos_y);
}

static void OnLeftArrow(Panel* p)
{
	if(p->_caret_idx > 0)
		--(p->_caret_idx);

	p->_UpdateCaretPosFunc(p);
}

static void OnRightArrow(Panel* p)
{
	if(p->_caret_idx <= p->_str_in->_len)
		++(p->_caret_idx);

	p->_UpdateCaretPosFunc(p);
}

static void OnChar(Panel* p, wchar_t ch)
{
	HideCaret(p->_hWndParent);

	String_insert_c(p->_str_in, p->_caret_idx, ch);

	p->_UpdateCaretPosFunc(p);

	ShowCaret(p->_hWndParent);
}
