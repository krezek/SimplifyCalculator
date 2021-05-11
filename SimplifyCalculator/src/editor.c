#include "platform.h"

#include "editor.h"
#include <parser.h>

static void OnInitialize(Editor* ed);
static void OnStopEditing(Editor* ed);

static void Update_ItemsOrder(Editor* ed);
static void TreeWalker(Editor* ed, Item** pItem);

EditorNode* EditorNode_init(Item** pIm, NodeType nt, String* str, int index, EditorNode* n, EditorNode* p)
{
	EditorNode* en = (EditorNode*)malloc(sizeof(EditorNode));
	assert(en);

	en->_pItem = pIm;
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

void LinkedList_pushpack(EditorLinkedList* ll, Item** pIm, NodeType nt, String* str, int index)
{
	if (ll->_rear == NULL)
	{
		assert(ll->_front == NULL);
		ll->_front = ll->_rear = EditorNode_init(pIm, nt, str, index, NULL, NULL);
	}
	else
	{
		EditorNode* i = EditorNode_init(pIm, nt, str, index, NULL, ll->_rear);
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
	ed->_OnStopEditingFunc = OnStopEditing;

	return ed;
}

void Editor_free(Editor* ed)
{
	if (ed->_itemsOrder)
	{
		LinkedList_free(ed->_itemsOrder);
		ed->_itemsOrder = NULL;
	}

	free(ed);
}

static void OnInitialize(Editor* ed)
{
	int rs = parse(ed->_pItems, L"x_1=(-b+sqrt(b^2-4*a*c))/(2*a),x_2=(-b-sqrt(b^2-4*a*c))/(2*a)");
	if (rs)
	{
		wprintf(L"Editor:OnInitialize:parse Error\n");
	}

	Update_ItemsOrder(ed);
}

static void OnStopEditing(Editor* ed)
{

}

static void Update_ItemsOrder(Editor* ed)
{
	if (ed->_itemsOrder)
	{
		LinkedList_free(ed->_itemsOrder);
	}

	ed->_itemsOrder = LinkedList_init();

	TreeWalker(ed, ed->_pItems);
}

void TreeWalker(Editor* ed, Item** pItem)
{
	if ((*pItem)->_objectType == OBJ_Add ||
		(*pItem)->_objectType == OBJ_Sub ||
		(*pItem)->_objectType == OBJ_Mult ||
		(*pItem)->_objectType == OBJ_Frac ||
		(*pItem)->_objectType == OBJ_Pow ||
		(*pItem)->_objectType == OBJ_Subscript ||
		(*pItem)->_objectType == OBJ_List ||
		(*pItem)->_objectType == OBJ_Equ)
	{
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Begin, NULL, 0);
		TreeWalker(ed, &(*pItem)->_left);
		TreeWalker(ed, &(*pItem)->_right);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_End, NULL, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Parentheses ||
		(*pItem)->_objectType == OBJ_Sign ||
		(*pItem)->_objectType == OBJ_Factorial ||
		(*pItem)->_objectType == OBJ_Sqrt ||
		(*pItem)->_objectType == OBJ_CommFunc)
	{
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Begin, NULL, 0);
		TreeWalker(ed, &(*pItem)->_left);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_End, NULL, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Sigma)
	{
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Begin, NULL, 0);
		if (((ItemSigma*)((*pItem)))->_bottom)
			TreeWalker(ed, &((ItemSigma*)((*pItem)))->_bottom);
		if (((ItemSigma*)((*pItem)))->_top)
			TreeWalker(ed, &((ItemSigma*)((*pItem)))->_top);
		TreeWalker(ed, &(*pItem)->_left);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_End, NULL, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Integrate)
	{
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Begin, NULL, 0);
		if (((ItemIntegrate*)((*pItem)))->_bottom)
			TreeWalker(ed, &((ItemIntegrate*)((*pItem)))->_bottom);
		if (((ItemIntegrate*)((*pItem)))->_top)
			TreeWalker(ed, &((ItemIntegrate*)((*pItem)))->_top);
		TreeWalker(ed, &(*pItem)->_left);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_End, NULL, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Number)
	{
		ItemNumber* i = (ItemNumber*)(*pItem);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Number, i->_str, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Literal)
	{
		ItemLiteral* i = (ItemLiteral*)(*pItem);
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Literal, i->_str, 0);
	}
	else if ((*pItem)->_objectType == OBJ_Base)
	{
		LinkedList_pushpack(ed->_itemsOrder, pItem, NT_Null, NULL, 0);
	}
	else
	{
		wprintf(L"Editor_TreeWalker: error\n");
	}
}

