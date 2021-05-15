#include "platform.h"

#include "editor.h"
#include "ids.h"


typedef Item* (*initFunc2param) (Item*, Item*);
typedef Item* (*initFunc1param) (Item*);

static void OnInit(Editor* ed);
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

static EditorNode* get_node(Editor* ed, Item* itm);
static EditorNode* get_next_node(Editor* ed, EditorNode* node);
static EditorNode* get_prev_node(Editor* ed, EditorNode* node);

Item* add_item_2param_right(Editor* ed, initFunc2param ifunc, Item** blank);
Item* add_item_2param_left(Editor* ed, initFunc2param ifunc, Item** blank);
Item* add_item_1param(Editor* ed, initFunc1param ifunc, Item** blank);
void add_necessary_parentheses_2param(Item** parent, Item** origin, Item** newItem);
void add_necessary_parentheses_1param(Item** parent, Item** origin, Item** newItem);

initFunc2param get_func_2param(const wchar_t ch);
initFunc1param get_func_1param(const wchar_t ch);

static void OnCmd(Editor* ed, int cmd);


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

	ed->_OnInitFunc = OnInit;
	ed->_OnStopEditingFunc = OnStopEditing;

	ed->_OnSetFocusFunc = OnSetFocus;
	ed->_OnKillFocusFunc = OnKillFocus;
	ed->_OnUpdateCaret = UpdateCaret;

	ed->_OnKey_LeftArrowFunc = OnKey_LeftArrow;
	ed->_OnKey_RightArrowFunc = OnKey_RightArrow;
	ed->_OnChar_DefaultFunc = OnChar_Default;
	ed->_OnChar_BackspaceFunc = OnChar_Backspace;
	ed->_OnChar_ReturnFunc = OnChar_Return;

	ed->_OnCmdFunc = OnCmd;

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

static void OnInit(Editor* ed)
{
	*ed->_pItems = Item_init(NULL, NULL);

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
	if (ed->_current_node->_nodeType == NT_Number ||
		ed->_current_node->_nodeType == NT_Literal)
	{
		if (ed->_current_node->_index > 0)
		{
			ed->_current_node->_index -= 1;
			UpdateCaret(ed);
			return;
		}
	}

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
	if (ed->_current_node->_nodeType == NT_Number ||
		ed->_current_node->_nodeType == NT_Literal)
	{
		if (ed->_current_node->_index < (int)ed->_current_node->_str->_len)
		{
			ed->_current_node->_index += 1;
			UpdateCaret(ed);
			return;
		}
	}

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
	Item* newItem = NULL;
	Item* blank = NULL;
	initFunc2param pf2 = NULL;
	initFunc1param pf1 = NULL;

	pf2 = get_func_2param(ch);
	if (pf2)
	{
		if ((ed->_current_node->_nodeType == NT_Null) ||
			(ed->_current_node->_nodeType == NT_End) ||
			(ed->_current_node->_nodeType == NT_Number) ||
			(ed->_current_node->_nodeType == NT_Literal))
		{
			newItem = add_item_2param_right(ed, pf2, &blank);
		}
		else if (ed->_current_node->_nodeType == NT_Begin)
		{
			newItem = add_item_2param_left(ed, pf2, &blank);
		}
	}
	else
	{
		pf1 = get_func_1param(ch);
		if (pf1)
		{
			newItem = add_item_1param(ed, pf1, &blank);
		}
	}

	wchar_t s[2];
	s[0] = ch;
	s[1] = 0;

	if (!newItem && ed->_current_node->_nodeType == NT_Null)
	{
		Item* parent = Item_getParent(*ed->_pItems, (*ed->_current_node->_pItem));
		if (!parent)
		{
			ItemTree_free(ed->_pItems);

			if (isdigit(ch))
				newItem = (Item*)ItemNumber_init(s, 0);
			else
				newItem = (Item*)ItemLiteral_init(s);

			*ed->_pItems = newItem;
		}
		else
		{
			if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
			{
				ItemTree_free(&parent->_left);
				if (isdigit(ch))
					newItem = parent->_left = (Item*)ItemNumber_init(s, 0);
				else
					newItem = parent->_left = (Item*)ItemLiteral_init(s);
			}
			else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
			{
				ItemTree_free(&parent->_right);
				if (isdigit(ch))
					newItem = parent->_right = (Item*)ItemNumber_init(s, 0);
				else
					newItem = parent->_right = (Item*)ItemLiteral_init(s);
			}
		}
	}
	else if (!newItem && (ed->_current_node->_nodeType == NT_Number ||
		ed->_current_node->_nodeType == NT_Literal))
	{
		String_insert_s(ed->_current_node->_str, ed->_current_node->_index, s);
		ed->_current_node->_index += 1;
	}

	if (newItem)
	{
		Update_ItemsOrder(ed);
		EditorNode* en = get_node(ed, newItem);
		ed->_current_node = en;
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 1);

		if (ed->_current_node->_nodeType == NT_Number ||
			ed->_current_node->_nodeType == NT_Literal)
		{
			ed->_current_node->_index += 1;
		}

		if (blank)
		{
			(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);
			EditorNode* en = get_node(ed, blank);
			ed->_current_node = en;
			(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 1);
		}
	}
}

static void OnChar_Backspace(Editor* ed)
{
	Item* newItem = NULL;

	if (ed->_current_node->_nodeType == NT_End)
	{
		if (ed->_current_node)
		{
			Item* parent = Item_getParent(*ed->_pItems, *ed->_current_node->_pItem);
			if (parent)
			{
				if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
				{
					ItemTree_free(&parent->_left);
					newItem = parent->_left = Item_init(NULL, NULL);
				}
				else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
				{
					ItemTree_free(&parent->_right);
					newItem = parent->_right = Item_init(NULL, NULL);
				}
			}
			else
			{
				ItemTree_free(ed->_pItems);
				newItem = (*ed->_pItems) = Item_init(NULL, NULL);
			}
		}
	}
	else if (ed->_current_node->_nodeType == NT_Number ||
		ed->_current_node->_nodeType == NT_Literal)
	{
		if (ed->_current_node->_index > 0)
		{
			ed->_current_node->_index -= 1;
			String_delete_c(ed->_current_node->_str, ed->_current_node->_index);
		}

		if (ed->_current_node->_str->_len == 0)
		{
			Item* parent = Item_getParent(*ed->_pItems, *ed->_current_node->_pItem);
			if (parent)
			{
				if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
				{
					ItemTree_free(&parent->_left);
					newItem = parent->_left = Item_init(NULL, NULL);
				}
				else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
				{
					ItemTree_free(&parent->_right);
					newItem = parent->_right = Item_init(NULL, NULL);
				}
			}
			else
			{
				ItemTree_free(ed->_pItems);
				newItem = (*ed->_pItems) = Item_init(NULL, NULL);
			}
		}
	}

	if (newItem)
	{
		Update_ItemsOrder(ed);
		EditorNode* en = get_node(ed, newItem);
		ed->_current_node = en;
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 1);
	}
}

static void OnChar_Return(Editor* ed)
{
}

static EditorNode* get_node(Editor* ed, Item* itm)
{
	if (ed->_itemsOrder)
	{
		if (ed->_itemsOrder->_front)
		{
			EditorNode* ln = ed->_itemsOrder->_front;
			while (ln)
			{
				if (*ln->_pItem == itm)
				{
					return ln;
				}

				ln = ln->_next;
			}
		}
	}

	return NULL;
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

void add_necessary_parentheses_2param(Item** parent, Item** origin, Item** newItem)
{
	if (*origin && (*origin)->_procLevel < (*newItem)->_procLevel)
	{
		if ((*newItem)->_objectType == OBJ_Mult)
		{
			if ((*newItem)->_left == (*origin))
			{
				(*newItem)->_left = (Item*)ItemParentheses_init(*origin);
			}
			else if ((*newItem)->_right == (*origin))
			{
				(*newItem)->_right = (Item*)ItemParentheses_init(*origin);
			}
		}
	}
	
	if (*parent && (*parent)->_procLevel > (*newItem)->_procLevel)
	{
		if ((*parent)->_objectType == OBJ_Mult)
		{
			*newItem = (Item*)ItemParentheses_init(*newItem);
		}
	}

	if (*parent && (*parent)->_procLevel == (*newItem)->_procLevel)
	{
		if ((*parent)->_objectType == OBJ_Sub &&
			(*newItem)->_objectType == OBJ_Add)
		{
			*newItem = (Item*)ItemParentheses_init(*newItem);
		}
	}
}

Item* add_item_2param_right(Editor*ed, initFunc2param ifunc, Item** blank)
{
	Item* newItem = NULL;

	if (ed->_current_node)
	{
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);

		Item* parent = Item_getParent(*ed->_pItems, *ed->_current_node->_pItem);
		if (parent)
		{
			if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(parent->_left, NULL);
				*blank = i->_right;
				add_necessary_parentheses_2param(&parent, &parent->_left, &i);
				newItem = parent->_left = i;
			}
			else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(parent->_right, NULL);
				*blank = i->_right;
				add_necessary_parentheses_2param(&parent, &parent->_right, &i);
				newItem = parent->_right = i;
			}
		}
		else
		{
			Item* i = ifunc(*ed->_pItems, NULL);
			*blank = i->_right;
			add_necessary_parentheses_2param(&parent, ed->_pItems, &i);
			newItem = *ed->_pItems = i;
			
		}
	}

	return newItem;
}

Item* add_item_2param_left(Editor* ed, initFunc2param ifunc, Item** blank)
{
	Item* newItem = NULL;

	if (ed->_current_node)
	{
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);

		Item* parent = Item_getParent(*ed->_pItems, *ed->_current_node->_pItem);
		if (parent)
		{
			if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(NULL, parent->_left);
				*blank = i->_left;
				add_necessary_parentheses_2param(&parent, &parent->_left, &i);
				newItem = parent->_left = i;
			}
			else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(NULL, parent->_right);
				*blank = i->_left;
				add_necessary_parentheses_2param(&parent, &parent->_right, &i);
				newItem = parent->_right = i;
			}
		}
		else
		{
			Item* i = ifunc(NULL, *ed->_pItems);
			*blank = i->_left;
			add_necessary_parentheses_2param(&parent, ed->_pItems, &i);
			newItem = *ed->_pItems = i;
		}
	}

	return newItem;
}

void add_necessary_parentheses_1param(Item** parent, Item** origin, Item** newItem)
{
	if (*origin && (*origin)->_procLevel < PROC_L_6 && (*newItem)->_objectType == OBJ_Factorial)
	{
		(*newItem)->_left = (Item*)ItemParentheses_init(*origin);
	}
}

Item* add_item_1param(Editor* ed, initFunc1param ifunc, Item** blank)
{
	Item* newItem = NULL;

	if (ed->_current_node)
	{
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);

		Item* parent = Item_getParent(*ed->_pItems, *ed->_current_node->_pItem);
		if (parent)
		{
			if (parent->_left && (parent->_left == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(parent->_left);
				*blank = i->_left;
				add_necessary_parentheses_1param(&parent, &parent->_left, &i);
				newItem = parent->_left = i;
			}
			else if (parent->_right && (parent->_right == *ed->_current_node->_pItem))
			{
				Item* i = ifunc(parent->_right);
				*blank = i->_left;
				add_necessary_parentheses_1param(&parent, &parent->_right, &i);
				newItem = parent->_right = i;
			}
		}
		else
		{
			Item* i = ifunc(*ed->_pItems);
			*blank = i->_left;
			add_necessary_parentheses_1param(&parent, ed->_pItems, &i);
			newItem = *ed->_pItems = i;			
		}
	}

	return newItem;
}

initFunc2param get_func_2param(const wchar_t ch)
{
	switch (ch)
	{
	case '+':
		return (initFunc2param)ItemAdd_init;
	case '-':
		return (initFunc2param)ItemSub_init;
	case '*':
		return (initFunc2param)ItemMult_init;
	case '/':
		return (initFunc2param)ItemFrac_init;
	case '^':
		return (initFunc2param)ItemPow_init;
	case '_':
		return (initFunc2param)ItemSubscript_init;

	default:
		return NULL;
	}
}

initFunc1param get_func_1param(const wchar_t ch)
{
	switch (ch)
	{
	case '!':
		return (initFunc1param)ItemFactorial_init;
	case '(':
	case ')':
		return (initFunc1param)ItemParentheses_init;
	case '$':
		return (initFunc1param)ItemSignPlus_init;
	case '#':
		return (initFunc1param)ItemSignMinus_init;
	case ':':
		return (initFunc1param)ItemSqrt_init;

	default:
		return NULL;
	}
}

static void OnCmd(Editor* ed, int cmd)
{
	Item* newItem = NULL;
	Item* blank = NULL;

	switch (cmd)
	{
	case cmdButtonSin:
		newItem = add_item_1param(ed, (initFunc1param)ItemSinFunc_init, &blank);
		break;
	case cmdButtonCos:
		newItem = add_item_1param(ed, (initFunc1param)ItemCosFunc_init, &blank);
		break;
	case cmdButtonTan:
		newItem = add_item_1param(ed, (initFunc1param)ItemTanFunc_init, &blank);
		break;
	case cmdButtonLog:
		newItem = add_item_1param(ed, (initFunc1param)ItemLogFunc_init, &blank);
		break;
	case cmdButtonExp:
		newItem = add_item_1param(ed, (initFunc1param)ItemExpFunc_init, &blank);
		break;
	case cmdButtonAsin:
		newItem = add_item_1param(ed, (initFunc1param)ItemAsinFunc_init, &blank);
		break;
	case cmdButtonAcos:
		newItem = add_item_1param(ed, (initFunc1param)ItemAcosFunc_init, &blank);
		break;
	case cmdButtonAtan:
		newItem = add_item_1param(ed, (initFunc1param)ItemAtanFunc_init, &blank);
		break;
	case cmdButtonLn:
		newItem = add_item_1param(ed, (initFunc1param)ItemLnFunc_init, &blank);
		break;
	default:
		break;
	}

	if (newItem)
	{
		Update_ItemsOrder(ed);
		EditorNode* en = get_node(ed, newItem);
		ed->_current_node = en;
		(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 1);

		if (ed->_current_node->_nodeType == NT_Number ||
			ed->_current_node->_nodeType == NT_Literal)
		{
			ed->_current_node->_index += 1;
		}

		if (blank)
		{
			(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 0);
			EditorNode* en = get_node(ed, blank);
			ed->_current_node = en;
			(*ed->_current_node->_pItem)->_setFocusFunc(*ed->_current_node->_pItem, 1);
		}
	}
}
