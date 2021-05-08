#ifndef _BASE_WND_H_
#define _BASE_WND_H_

typedef struct _BaseWindow BaseWindow;

typedef BOOL (*CreateFunc)(BaseWindow* _this);
typedef void (*AfterCreateFunc) (BaseWindow* _this);
typedef LRESULT (*HandleMessageFunc) (BaseWindow* _this, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef BOOL (*ShowFunc)(BaseWindow* _this, int nCmdShow);
typedef BOOL (*UpdateFunc)(BaseWindow* _this);
typedef BOOL (*OnSizingFunc)(BaseWindow* _this, RECT* pRect);

typedef struct _BaseWindow
{
	HWND _hWnd;

	int _minWidth, _minHeight;

	CreateFunc _CreateFunc;
	AfterCreateFunc _AfterCreateFunc;
	HandleMessageFunc _HandleMessageFunc;
	ShowFunc _ShowFunc;
	UpdateFunc _UpdateFunc;
	OnSizingFunc _OnSizingFunc;
} BaseWindow;

LRESULT CALLBACK BaseWindow_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void BaseWindow_default(BaseWindow* _this);

void ShowError(const wchar_t* lpszFunction);

#endif /* _BASE_WND_H_*/

