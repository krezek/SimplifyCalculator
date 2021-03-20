#include "platform.h"

#include "panel.h"

static const int g_panel_margin_h = 10, g_panel_margin_v = 10;

static void OnInitialze(PanelLinkedList* pll);
static void DrawList(PanelLinkedList* pll, HDC hdc);
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

	pll->_OnInitializeFunc = OnInitialze;
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
	p1->_height = 150;
	PanelLinkedList_pushpack(pll, p1);

	Panel* p2 = Panel_init();
	p2->_height =175;
	PanelLinkedList_pushpack(pll, p2);

	Panel* p3 = Panel_init();
	p3->_height = 250;
	PanelLinkedList_pushpack(pll, p3);
}

static void DrawList(PanelLinkedList* pll, HDC hdc)
{
	int x = g_panel_margin_h;
	int y = g_panel_margin_v;

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

	p->_DrawFunc = Draw;

	return p;
}

void Panel_free(Panel* p)
{
	free(p);
}

static void Draw(Panel* p, HDC hdc)
{
	RECT rc;
	rc.left = p->_x;
	rc.top = p->_y;
	rc.right = rc.left + p->_width;
	rc.bottom = rc.top + p->_height;

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOWFRAME));
}

