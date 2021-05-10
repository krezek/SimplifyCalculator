#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <items.h>
#include <strg.h>

typedef enum { NT_Null, NT_Begin, NT_End, NT_Number, NT_Literal } NodeType;
typedef struct _Editor Editor;

typedef void (*OnEditorInitializeFunc) (Editor* ed);

typedef struct _EditorNode
{
	Item* _item;

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
} Editor;

Editor* Editor_init(Item** pitems);
void Editor_free(Editor* ed);


#endif /* _EDITOR_H_ */

