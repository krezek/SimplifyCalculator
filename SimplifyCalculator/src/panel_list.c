#include "platform.h"

#include "panel_list.h"

static void OnInit(PanelList* pll, HWND hWnd);
static Panel* AddNewPanel(PanelList* pll);
static void PosChanged(PanelList* pll);
static void OnFontChanged(PanelList* pll);
static int GetViewportWidth(PanelList* pll);
static int GetViewportHeight(PanelList* pll);
static void OnPaint(PanelList* pll, HDC hdc, RECT* rcPaint);

static void SetCntSize(PanelList* pll);

extern HFONT g_math_font;

static const int g_margin_h = 10, g_margin_v = 10;

const wchar_t* g_in_str = L"In:";
const wchar_t* g_out_str = L"Out:";

int g_in_str_width, g_in_str_height;
int g_out_str_width, g_out_str_height;
int g_padding;

PanelNode* PanelNode_init(Panel* p, PanelNode* nxt, PanelNode* prv)
{
	PanelNode* pn = (PanelNode*)malloc(sizeof(PanelNode));
	assert(pn);

	pn->_panel = p;
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

PanelList* PanelList_init()
{
	PanelList* pll = (PanelList*)malloc(sizeof(PanelList));
	assert(pll != NULL);

	pll->_front = NULL;
	pll->_rear = NULL;

	pll->_selected_panel = NULL;

	pll->_width = pll->_height = 0;
	pll->_x_current_pos = pll->_y_current_pos = 0;

	pll->_OnInitFunc = OnInit;
	pll->_PosChangedFunc = PosChanged;
	pll->_OnFontChangedFunc = OnFontChanged;
	pll->_GetViewportWidthFunc = GetViewportWidth;
	pll->_GetViewportHeightFunc = GetViewportHeight;
	pll->_OnPaintFunc = OnPaint;
	pll->_AddNewPanelFunc = AddNewPanel;

	return pll;
}

void PanelList_free(PanelList* pll)
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

void PanelList_pushpack(PanelList* pll, Panel* p)
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

static void OnInit(PanelList* pll, HWND hWnd)
{
	pll->_hWndParent = hWnd;
	SetCntSize(pll);
}

static void SetCntSize(PanelList* pll)
{
	SIZE sz1, sz2, sz3;

	HDC hdc = GetDC(pll->_hWndParent);
	SelectObject(hdc, g_math_font);

	GetTextExtentPoint(hdc, g_in_str, (int)wcslen(g_in_str), &sz1);
	GetTextExtentPoint(hdc, g_out_str, (int)wcslen(g_out_str), &sz2);
	GetTextExtentPoint(hdc, L"W", 1, &sz3);

	g_in_str_width = sz1.cx;
	g_in_str_height = sz1.cy;

	g_out_str_width = sz2.cx;
	g_out_str_height = sz2.cy;

	g_padding = sz3.cx;

	ReleaseDC(pll->_hWndParent, hdc);
}

static Panel* AddNewPanel(PanelList* pll)
{
	int x, y;

	x = pll->_x0 + g_margin_v;
	y = (pll->_rear) ? (pll->_rear->_panel->_y0 + pll->_rear->_panel->_height + g_margin_h) : pll->_y0 + g_margin_h;
	x -= pll->_x_current_pos;
	y -= pll->_y_current_pos;

	Panel* p = Panel_init(pll->_hWndParent);
	PanelList_pushpack(pll, p);
	pll->_selected_panel = p;

	p->_x0 = x;
	p->_y0 = y;

	p->_OnInitFunc(p);
	
	return p;
}

static void PosChanged(PanelList* pll)
{
	int x = pll->_x0 + g_margin_h - pll->_x_current_pos;
	int y = pll->_y0 + g_margin_v - pll->_y_current_pos;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_x0 = x;
				pn->_panel->_y0 = y;

				pn->_panel->_PosChangedFunc(pn->_panel);

				y += pn->_panel->_height + g_margin_v;

				pn = pn->_next;
			}
		}
	}
}

static void OnFontChanged(PanelList* pll)
{
	int x = pll->_x0 + g_margin_h - pll->_x_current_pos;
	int y = pll->_y0 + g_margin_v - pll->_y_current_pos;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				pn->_panel->_x0 = x;
				pn->_panel->_y0 = y;

				pn->_panel->_OnFontChangedFunc(pn->_panel);

				y += pn->_panel->_height + g_margin_v;

				pn = pn->_next;
			}
		}
	}
}

static int GetViewportWidth(PanelList* pll)
{
	int w = 0;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				w = max(w, pn->_panel->_width);
				pn = pn->_next;
			}
		}
	}

	w += g_margin_v * 2;

	return w;
}

static int GetViewportHeight(PanelList* pll)
{
	int y = g_margin_v;

	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				y += pn->_panel->_height + g_margin_v;
				pn = pn->_next;
			}
		}
	}

	return y;
}

static void OnPaint(PanelList* pll, HDC hdc, RECT* rcPaint)
{
	if (pll)
	{
		if (pll->_front)
		{
			PanelNode* pn = pll->_front;
			while (pn)
			{
				Panel* p = pn->_panel;
				RECT rc, pRect;

				pRect.left = p->_x0;
				pRect.top = p->_y0;
				pRect.right = pRect.left + p->_width;
				pRect.bottom = pRect.top + p->_height;

				if (IntersectRect(&rc, rcPaint, &pRect))
				{
					p->_OnPaintFunc(pn->_panel, hdc);
				}

				pn = pn->_next;
			}
		}
	}
}
