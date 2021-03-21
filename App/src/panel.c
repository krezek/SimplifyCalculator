#include "platform.h"

#include "panel.h"

extern HFONT g_bold_font, g_math_font, g_fixed_font;

static const int g_panel_margin_h = 10, g_panel_margin_v = 10;
static const int g_content_margin_h = 10, g_content_margin_v = 10;
static const int g_padding = 30;

static void OnInitialze(PanelLinkedList* pll);
static void ParentSizeChanged(PanelLinkedList* pll);
static int GetViewportHeight(PanelLinkedList* pll);
static void DrawList(PanelLinkedList* pll, HDC hdc);
static void CalcHeight(Panel* p);
static void Draw(Panel* p, HDC hdc);

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
	pll->_ParentSizeChangedFunc = ParentSizeChanged;
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
	Panel* p1 = Panel_init();
	p1->_hWndParent = pll->_hWndParent;
	String_cpy(p1->_cnt_str_in, L"In:");
	String_cpy(p1->_str_in, L"x_1=(-b+sqrt(b^2-4*a*c))/(2*a),x_2=(-b-sqrt(b^2-4*a*c))/(2*a)");
	String_cpy(p1->_cnt_str_out, L"Value:");
	PanelLinkedList_pushpack(pll, p1);

	Panel* p2 = Panel_init();
	p2->_hWndParent = pll->_hWndParent;
	String_cpy(p2->_cnt_str_in, L"In:");
	String_cpy(p2->_str_in, L"n_0*x^n+n_1*x^(n-1)+n_2*x^(n-2)+tdot+n_(k-2)*x^2+n_(k-1)*x+n_k=0");
	String_cpy(p2->_cnt_str_out, L"Value:");
	PanelLinkedList_pushpack(pll, p2);

	Panel* p3 = Panel_init();
	p3->_hWndParent = pll->_hWndParent;
	String_cpy(p3->_cnt_str_in, L"In:");
	String_cpy(p3->_str_in, L"Cos(x;2)+Sin(x;2)=1");
	String_cpy(p3->_cnt_str_out, L"Value:");
	PanelLinkedList_pushpack(pll, p3);
}

static void ParentSizeChanged(PanelLinkedList* pll)
{
	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_width = pll->_client_width - g_panel_margin_h * 2;
				pn->_panel->_CalcHeightFunc(pn->_panel);
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

				y += pn->_panel->_height + g_panel_margin_v;

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
	p->_DrawFunc = Draw;

	return p;
}

void Panel_free(Panel* p)
{
	String_free(p->_cnt_str_in);
	String_free(p->_cnt_str_out);
	String_free(p->_str_in);
	String_free(p->_str_out);

	free(p);
}

static void CalcHeight(Panel* p)
{
	HDC hdc = GetDC(p->_hWndParent);

	TEXTMETRIC tmFixed; 
	SIZE s1;
	
	HGDIOBJ hOldFont = SelectObject(hdc, g_bold_font);
	GetTextExtentPoint32(hdc, p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len, &s1);

	SelectObject(hdc, g_fixed_font);
	GetTextMetrics(hdc, &tmFixed);
	int w1 = p->_width - g_content_margin_h * 2 - s1.cx - g_padding;
	int col1 = w1 / tmFixed.tmAveCharWidth;
	int row_count = (int)(p->_str_in->_len / col1) + (p->_str_in->_len % col1 > 0 ? 1 : 0);

	p->_height = row_count * tmFixed.tmHeight + 2 * g_content_margin_v;

	ReleaseDC(p->_hWndParent, hdc);
}

static void Draw(Panel* p, HDC hdc)
{
	TEXTMETRIC tmFixed;
	SIZE s1;
	RECT rc;
	rc.left = p->_x;
	rc.top = p->_y;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));

	// Draw cnt_in_str "In:"
	HGDIOBJ hOldFont = SelectObject(hdc, g_bold_font);
	GetTextExtentPoint32(hdc, p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len, &s1);
	TextOut(hdc, p->_x + g_content_margin_h, p->_y + g_content_margin_v,
		p->_cnt_str_in->_str, (int)p->_cnt_str_in->_len);

	{
		SelectObject(hdc, g_fixed_font);
		GetTextMetrics(hdc, &tmFixed);
		int w1 = p->_width - g_content_margin_h * 2 - s1.cx - g_padding;
		int col1 = w1 / tmFixed.tmAveCharWidth;

		int iy = 0;
		int ix = 0;
		while (ix < p->_str_in->_len)
		{
			int v = ix % col1;
			TextOut(hdc, p->_x + g_content_margin_h + s1.cx + g_padding + v * tmFixed.tmAveCharWidth,
				p->_y + g_content_margin_v + iy * tmFixed.tmHeight, p->_str_in->_str + ix, 1);
			if (v == col1 - 1)
				++iy;
			++ix;
		}
	}

	SelectObject(hdc, hOldFont);
}

