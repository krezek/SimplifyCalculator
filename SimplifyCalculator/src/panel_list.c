#include "platform.h"

#include "panel_list.h"
#include <parser.h>

static void OnInitialze(PanelLinkedList* pll, HWND hWnd);
static Panel* AddNewPanel(PanelLinkedList* pll);
static void ParentPosChanged(PanelLinkedList* pll);
static void ParentFontChanged(PanelLinkedList* pll);
static int GetViewportWidth(PanelLinkedList* pll);
static int GetViewportHeight(PanelLinkedList* pll);
static void DrawList(PanelLinkedList* pll, HDC hdc, RECT* rcPaint);

static const int g_margin_h = 10, g_margin_v = 10;
extern HFONT g_math_font;

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

PanelLinkedList* PanelLinkedList_init()
{
	PanelLinkedList* pll = (PanelLinkedList*)malloc(sizeof(PanelLinkedList));
	assert(pll != NULL);

	pll->_front = NULL;
	pll->_rear = NULL;

	pll->_selected_panel = NULL;

	pll->_width = pll->_height = 0;
	pll->_x_current_pos = pll->_y_current_pos = 0;

	pll->_OnInitializeFunc = OnInitialze;
	pll->_ParentPosChangedFunc = ParentPosChanged;
	pll->_ParentFontChangedFunc = ParentFontChanged;
	pll->_GetViewportWidthFunc = GetViewportWidth;
	pll->_GetViewportHeightFunc = GetViewportHeight;
	pll->_DrawListFunc = DrawList;
	pll->_AddNewPanelFunc = AddNewPanel;

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

static void OnInitialze(PanelLinkedList* pll, HWND hWnd)
{
	pll->_hWndParent = hWnd;
}

static Panel* AddNewPanel(PanelLinkedList* pll)
{
	int x, y;

	x = pll->_x0 + g_margin_v;
	y = (pll->_rear) ? (pll->_rear->_panel->_y0 + pll->_rear->_panel->_height + g_margin_h) : pll->_y0 + g_margin_h;
	x -= pll->_x_current_pos;
	y -= pll->_y_current_pos;

	Panel* p = Panel_init(pll->_hWndParent);
	PanelLinkedList_pushpack(pll, p);
	pll->_selected_panel = p;

	p->_x0 = x;
	p->_y0 = y;

	p->_OnPanelInitFunc(p);
	
	return p;
}

static void ParentPosChanged(PanelLinkedList* pll)
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

static void ParentFontChanged(PanelLinkedList* pll)
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

				pn->_panel->_FontChangedFunc(pn->_panel);

				y += pn->_panel->_height + g_margin_v;

				pn = pn->_next;
			}
		}
	}
}

static int GetViewportWidth(PanelLinkedList* pll)
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

static int GetViewportHeight(PanelLinkedList* pll)
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

static void DrawList(PanelLinkedList* pll, HDC hdc, RECT* rcPaint)
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
					p->_DrawFunc(pn->_panel, hdc);
				}

				pn = pn->_next;
			}
		}
	}
}
