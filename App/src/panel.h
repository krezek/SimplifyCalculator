#ifndef _PANEL_H_
#define _PANEL_H_

typedef struct _Panel Panel;
typedef struct _PanelLinkedList PanelLinkedList;

typedef void (*OnInitializeFunc)(PanelLinkedList* pll);
typedef void (*DrawListFunc)(PanelLinkedList * pll, HDC hdc);
typedef void (*DrawFunc)(Panel* p, HDC hdc);

typedef struct _Panel
{
	int _x, _y, _width, _height;

	DrawFunc _DrawFunc;
} Panel;

typedef struct _PanelNode
{
	Panel* _panel;

	struct _PanelNode* _next;
	struct _PanelNode* _prev;
} PanelNode;

typedef struct _PanelLinkedList
{
	PanelNode* _front;
	PanelNode* _rear;

	OnInitializeFunc _OnInitializeFunc;
	DrawListFunc _DrawListFunc;
} PanelLinkedList;

Panel* Panel_init();
void Panel_free(Panel* p);

PanelNode* PanelNode_init(Panel* p, PanelNode* nxt, PanelNode* prv);
void PanelNode_free(PanelNode* pn);

PanelLinkedList* PanelLinkedList_init();
void PanelLinkedList_free(PanelLinkedList* pll);
void PanelLinkedList_pushpack(PanelLinkedList* pll, Panel* p);

#endif /* _PANEL_H_ */

