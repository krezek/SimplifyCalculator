#include "platform.h"

#include "editor.h"
#include <parser.h>

static void OnInitialize(Editor* ed);
static void OnStopEditing(Editor* ed);

static void UpdateCaret(Editor* ed);
static void OnSetFocus(Editor* ed);
static void OnKillFocus(Editor* ed);

static void OnKey_LeftArrow(Editor* ed);
static void OnKey_RightArrow(Editor* ed);
static void OnChar_Default(Editor* ed, wchar_t ch);
static void OnChar_Backspace(Editor* ed);
static void OnChar_Return(Editor* ed);

static void Update_ItemsOrder(Editor* ed);
static void TreeWalker(Editor* ed, Item** pItem);

static EditorNode* get_next_node(Editor* ed, EditorNode* node);
static EditorNode* get_prev_node(Editor* ed, EditorNode* node);

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

	ed->_OnSetFocusFunc = OnSetFocus;
	ed->_OnKillFocusFunc = OnKillFocus;

	ed->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	ed->_OnKey_RightArrowFunc = OnKey_RightArrow;
	ed->_OnChar_DefaultFunc = OnChar_Default;
	ed->_OnChar_BackspaceFunc = OnChar_Backspace;
	ed->_OnChar_ReturnFunc = OnChar_Return;

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
	if (ed->_itemsOrder->_front)
	{
		ed->_current_node = ed->_itemsOrder->_front;
		(*ed->_current_node->_pItem)->_setFocusFunc((*ed->_current_node->_pItem), 1);
	}
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

static void UpdateCaret(Editor* ed)
{
	CreateCaret(ed->_hWnd, (HBITMAP)NULL, 2, (*ed->_current_node->_pItem)->_height);

	if (ed->_current_node->_nodeType == NT_Begin ||
		ed->_current_node->_nodeType == NT_Null)
	{
		SetCaretPos((*ed->_current_node->_pItem)->_x, 
			(*ed->_current_node->_pItem)->_y - (*ed->_current_node->_pItem)->_baseLine);
	}
	else if (ed->_current_node->_nodeType == NT_End)
	{
		SetCaretPos((*ed->_current_node->_pItem)->_x + (*ed->_current_node->_pItem)->_width, 
			(*ed->_current_node->_pItem)->_y - (*ed->_current_node->_pItem)->_baseLine);
	}
	else if (ed->_current_node->_nodeType == NT_Number ||
		ed->_current_node->_nodeType == NT_Literal)
	{
		SIZE s;

		HDC hdc = GetDC(ed->_hWnd);
		SelectObject(hdc, (*ed->_current_node->_pItem)->_hFont._hfont);
		GetTextExtentPoint32(hdc, ed->_current_node->_str->_str, ed->_current_node->_index, &s);
		ReleaseDC(ed->_hWnd, hdc);

		SetCaretPos((*ed->_current_node->_pItem)->_x + s.cx, 
			(*ed->_current_node->_pItem)->_y - (*ed->_current_node->_pItem)->_baseLine);
	}

	ShowCaret(ed->_hWnd);
}

static void OnSetFocus(Editor* ed)
{
	UpdateCaret(ed);
}

static void OnKillFocus(Editor* ed)
{
	DestroyCaret();
}

static void OnKey_LeftArrow(Editor* ed)
{
	EditorNode* prevNode = get_prev_node(ed, ed->_current_node);

	if (!prevNode)
		return;

	(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);
	(*prevNode->_pItem)->_setFocusFunc(*prevNode->_pItem, 1);
	ed->_current_node = prevNode;

	UpdateCaret(ed);
}

static void OnKey_RightArrow(Editor* ed)
{
	EditorNode* nextNode = get_next_node(ed, ed->_current_node);

	if (!nextNode)
		return;

	(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);
	(*nextNode->_pItem)->_setFocusFunc(*nextNode->_pItem, 1);
	ed->_current_node = nextNode;

	UpdateCaret(ed);
}

static void OnChar_Default(Editor* ed, wchar_t ch)
{
}

static void OnChar_Backspace(Editor* ed)
{
}

static void OnChar_Return(Editor* ed)
{
}

EditorNode* get_next_node(Editor* ed, EditorNode* node)
{
	if (ed->_itemsOrder)
	{
		if (ed->_itemsOrder->_front)
		{
			EditorNode* ln = ed->_itemsOrder->_front;
			while (ln)
			{
				if (ln == node)
				{
					return ln->_next;
				}

				ln = ln->_next;
			}
		}
	}

	return NULL;
}

EditorNode* get_prev_node(Editor* ed, EditorNode* node)
{
	if (ed->_itemsOrder)
	{
		if (ed->_itemsOrder->_rear)
		{
			EditorNode* ln = ed->_itemsOrder->_rear;
			while (ln)
			{
				if (ln == node)
				{
					return ln->_prev;
				}

				ln = ln->_prev;
			}
		}
	}

	return NULL;
}
