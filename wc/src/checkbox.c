#include "platform.h"

#include <checkbox.h>

static BOOL Create(BaseWindow* _this);

CheckBox* CheckBox_init()
{
    CheckBox* wnd = (CheckBox*)malloc(sizeof(CheckBox));
    assert(wnd != NULL);

    BaseWindow_default((BaseWindow*)wnd);

    //wnd->_baseWindow._HandleMessageFunc = HandleMessage;
    wnd->_baseWindow._CreateFunc = Create;

    return wnd;
}

void CheckBox_free(CheckBox* wnd)
{
    free(wnd);
}

static BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindowEx(
        0,
        WC_BUTTON,
        (PCTSTR)NULL,
        SBARS_SIZEGRIP |
        BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_TABSTOP,
        0, 0, 0, 0,
        _this->_hWndParent,
        _this->_id,
        NULL,
        NULL);
    assert(hWnd != NULL);

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}
