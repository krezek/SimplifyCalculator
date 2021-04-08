#include "platform.h"

#include "panel_list.h"

static void OnInitialze(PanelLinkedList* pll);
static Panel* AddNewPanel(PanelLinkedList* pll);
static void ParentPropertyChanged(PanelLinkedList* pll);
static void ParentPosChanged(PanelLinkedList* pll);
static int GetViewportHeight(PanelLinkedList* pll);
static void DrawList(PanelLinkedList* pll, HDC hdc, RECT* rcPaint);

extern HFONT g_bold_font, g_math_font, g_fixed_font;
static const int g_panel_margin_h = 10, g_panel_margin_v = 10;


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

	pll->_client_width = pll->_client_height = 0;
	pll->_x_current_pos = pll->_y_current_pos = 0;

	pll->_OnInitializeFunc = OnInitialze;
	pll->_ParentPropertyChangedFunc = ParentPropertyChanged;
	pll->_ParentPosChangedFunc = ParentPosChanged;
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

static void OnInitialze(PanelLinkedList* pll)
{
	AddNewPanel(pll);
}

static Panel* AddNewPanel(PanelLinkedList* pll)
{
	Panel* p = Panel_init(pll->_hWndParent, L"In:", L"Value:");
	PanelLinkedList_pushpack(pll, p);

	pll->_selected_panel = p;
	return p;
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

				pRect.left = p->_x;
				pRect.top = p->_y;
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
