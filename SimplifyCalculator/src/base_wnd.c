#include "platform.h"
#include "base_wnd.h"

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

	if ((nWidth < _this->_minWidth) || (nHeight < _this->_minHeight))
	{
		pRect->right = max(pRect->right, pRect->left + _this->_minWidth);
		pRect->bottom = max(pRect->bottom, pRect->top + _this->_minHeight);
	}

	return TRUE;
}

void BaseWindow_default(BaseWindow* _this)
{
	_this->_minWidth = 300;
	_this->_minHeight = 400;

	_this->_CreateFunc = NULL;
	_this->_HandleMessageFunc = NULL;
	
	_this->_ShowFunc = BaseWindow_Show;
	_this->_UpdateFunc = BaseWindow_Update;
	_this->_OnSizingFunc = BaseWindow_OnSizing;
}

