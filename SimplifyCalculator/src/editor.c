#include "platform.h"

#include "editor.h"
#include <parser.h>

static void OnInitialize(Editor* ed);

EditorNode* EditorNode_init(Item* itm, NodeType nt, String* str, int index, EditorNode* n, EditorNode* p)
{
	EditorNode* en = (EditorNode*)malloc(sizeof(EditorNode));
	assert(en);

	en->_item = itm;
	en->_nodeType = nt;
	en->_str = str;
	en->_index = index;
	en->_next = n;
	en->_prev = p;

	return en;
}

void EditorNode_free(EditorNode* ln)
{
	free(ln);
}

EditorLinkedList* LinkedList_init()
{
	EditorLinkedList* ll = (EditorLinkedList*)malloc(sizeof(EditorLinkedList));
	assert(ll != NULL);

	ll->_front = NULL;
	ll->_rear = NULL;

	return ll;
}

void LinkedList_pushpack(EditorLinkedList* ll, Item* itm, NodeType nt, String* str, int index)
{
	if (ll->_rear == NULL)
	{
		assert(ll->_front == NULL);
		ll->_front = ll->_rear = EditorNode_init(itm, nt, str, index, NULL, NULL);
	}
	else
	{
		EditorNode* i = EditorNode_init(itm, nt, str, index, NULL, ll->_rear);
		ll->_rear->_next = i;
		ll->_rear = i;
	}
}

void LinkedList_free(EditorLinkedList* ll)
{
	if (ll)
	{
		if (ll->_front)
		{
			while (ll->_front)
			{
				EditorNode* ln = ll->_front;
				ll->_front = ll->_front->_next;

				EditorNode_free(ln);
			}

			ll->_front = NULL;
			ll->_rear = NULL;
		}

		free(ll);
	}
}

Editor* Editor_init(Item** pItems)
{
	Editor* ed = (Editor*)malloc(sizeof(Editor));
	assert(ed != NULL);

	ed->_hWnd = NULL;
	ed->_pItems = pItems;
	ed->_itemsOrder = NULL;
	ed->_current_node = NULL;

	ed->_OnEditorInitializeFunc = OnInitialize;

	return ed;
}

void Editor_free(Editor* ed)
{
	free(ed);
}

static void OnInitialize(Editor* ed)
{
	int rs = parse(ed->_pItems, L"x_1=(-b+sqrt(b^2-4*a*c))/(2*a),x_2=(-b-sqrt(b^2-4*a*c))/(2*a)");
	if (rs)
	{
		wprintf(L"Editor:OnInitialize:parse Error\n");
	}
}
