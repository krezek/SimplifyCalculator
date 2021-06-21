#ifndef _BASE_WND_H_
#define _BASE_WND_H_

#include <cmp.h>

typedef struct _BaseWindow BaseWindow;

typedef void(*SetParentFunc)(BaseWindow* _this, HWND hWndParent);
typedef void(*SetIdFunc)(BaseWindow* _this, void* id);
typedef BOOL (*CreateFunc)(BaseWindow* _this);
typedef LRESULT (*HandleMessageFunc) (BaseWindow* _this, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef BOOL (*ShowFunc)(BaseWindow* _this, int nCmdShow);
typedef BOOL (*UpdateFunc)(BaseWindow* _this);
typedef BOOL (*OnSizingFunc)(BaseWindow* _this, RECT* pRect);
typedef LRESULT(*SendMessageFunc)(BaseWindow* _this, UINT msg, WPARAM wParam, LPARAM lParam);
typedef BOOL(*MoveWindowFunc)(BaseWindow* _this, int x, int y, int width, int height, BOOL repaint);

typedef struct _BaseWindow
{
	Component _cmp;

	HWND _hWnd;
	HWND _hWndParent;
	void* _id;

	SetParentFunc _SetParentFunc;
	SetIdFunc _SetIdFunc;
	CreateFunc _CreateFunc;
	HandleMessageFunc _HandleMessageFunc;
	ShowFunc _ShowFunc;
	UpdateFunc _UpdateFunc;
	OnSizingFunc _OnSizingFunc;
	SendMessageFunc _SendMessageFunc;
	MoveWindowFunc _MoveWindowFunc;
} BaseWindow;

LRESULT CALLBACK BaseWindow_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void BaseWindow_default(BaseWindow* _this);

void ShowError(const wchar_t* lpszFunction);

#endif /* _BASE_WND_H_*/

