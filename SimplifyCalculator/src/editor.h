#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <items.h>
#include <strg.h>

typedef enum { NT_Null, NT_Begin, NT_End, NT_Number, NT_Literal } NodeType;
typedef struct _Editor Editor;

typedef void (*OnEditorInitializeFunc) (Editor* ed);
typedef void (*OnStopEditingFunc) (Editor* ed);

typedef void (*OnEditorSetFocusFunc)(Editor* ed);
typedef void (*OnEditorKillFocusFunc)(Editor* ed);

typedef void (*OnKey_EditorLeftArrowFunc)(Editor* ed);
typedef void (*OnKey_EditorRightArrowFunc)(Editor* ed);
typedef void (*OnChar_EditorDefaultFunc)(Editor* ed, wchar_t ch);
typedef void (*OnChar_EditorBackspaceFunc)(Editor* ed);
typedef void (*OnChar_EditorReturnFunc)(Editor* ed);

typedef struct _EditorNode
{
	Item** _pItem;

	NodeType _nodeType;
	String* _str;
	int _index;

	struct _EditorNode* _next;
	struct _EditorNode* _prev;
} EditorNode;

typedef struct
{
	EditorNode* _front;
	EditorNode* _rear;
} EditorLinkedList;

typedef struct _Editor
{
	HWND _hWnd;
	Item** _pItems;
	EditorLinkedList* _itemsOrder;
	EditorNode* _current_node;

	OnEditorInitializeFunc _OnEditorInitializeFunc;
	OnStopEditingFunc _OnStopEditingFunc;

	OnEditorSetFocusFunc _OnSetFocusFunc;
	OnEditorKillFocusFunc _OnKillFocusFunc;

	OnKey_EditorLeftArrowFunc _OnKey_LeftArrowFunc;
	OnKey_EditorRightArrowFunc _OnKey_RightArrowFunc;
	OnChar_EditorDefaultFunc _OnChar_DefaultFunc;
	OnChar_EditorBackspaceFunc _OnChar_BackspaceFunc;
	OnChar_EditorReturnFunc _OnChar_ReturnFunc;
} Editor;

Editor* Editor_init(Item** pitems);
void Editor_free(Editor* ed);


#endif /* _EDITOR_H_ */

