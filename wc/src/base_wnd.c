#include "platform.h"
#include <base_wnd.h>

void BaseWindow_SetParent(BaseWindow* _this, HWND hWndParent)
{
	_this->_hWndParent = hWndParent;
}

void BaseWindow_SetId(BaseWindow* _this, void* id)
{
	_this->_id = id;
}

LRESULT BaseWindow_SendMessage(BaseWindow* _this, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(_this->_hWnd, msg, wParam, lParam);
}

BOOL BaseWindow_MoveWindow(BaseWindow* _this, int x, int y, int width, int height, BOOL repaint)
{
	_this->_cmp._x = x;
	_this->_cmp._y = y;
	_this->_cmp._width = width;
	_this->_cmp._height = height;

	return MoveWindow(_this->_hWnd, x, y, width, height, repaint);
}

LRESULT CALLBACK BaseWindow_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseWindow* pThis;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (BaseWindow*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->_hWnd = hWnd;
	}
	else
	{
		pThis = (BaseWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (pThis)
	{
		return pThis->_HandleMessageFunc(pThis, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

BOOL BaseWindow_Show(BaseWindow* _this, int nCmdShow)
{
	return ShowWindow(_this->_hWnd, nCmdShow);
}

BOOL BaseWindow_Update(BaseWindow* _this)
{
	return UpdateWindow(_this->_hWnd);
}

BOOL BaseWindow_OnSizing(BaseWindow* _this, RECT* pRect)
{
	LONG nWidth = pRect->right - pRect->left;
	LONG nHeight = pRect->bottom - pRect->top;

	if ((nWidth < _this->_cmp._minWidth) || (nHeight < _this->_cmp._minHeight))
	{
		pRect->right = max(pRect->right, pRect->left + _this->_cmp._minWidth);
		pRect->bottom = max(pRect->bottom, pRect->top + _this->_cmp._minHeight);
	}

	return TRUE;
}

void BaseWindow_default(BaseWindow* _this)
{
	Component_default((Component*)_this);

	_this->_cmp._type = CMP_WINDOW;

	_this->_cmp._minWidth = 0;
	_this->_cmp._minHeight = 0;

	_this->_SetParentFunc = BaseWindow_SetParent;
	_this->_SetIdFunc = BaseWindow_SetId;

	_this->_CreateFunc = NULL;
	_this->_HandleMessageFunc = NULL;
	
	_this->_ShowFunc = BaseWindow_Show;
	_this->_UpdateFunc = BaseWindow_Update;
	_this->_OnSizingFunc = BaseWindow_OnSizing;
	_this->_SendMessageFunc = BaseWindow_SendMessage;
	_this->_MoveWindowFunc = BaseWindow_MoveWindow;
}

void ShowError(const wchar_t* lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
