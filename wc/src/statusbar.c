#include "platform.h"

#include <statusbar.h>

static BOOL Create(BaseWindow* _this);

StatusBar* StatusBar_init()
{
    StatusBar* wnd = (StatusBar*)malloc(sizeof(StatusBar));
    assert(wnd != NULL);

    BaseWindow_default((BaseWindow*)wnd);

    //wnd->_baseWindow._HandleMessageFunc = HandleMessage;
    wnd->_baseWindow._CreateFunc = Create;

    return wnd;
}

void StatusBar_free(StatusBar* wnd)
{
    free(wnd);
}

static BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindowEx(
        0,
        STATUSCLASSNAME,
        (PCTSTR)NULL,
        SBARS_SIZEGRIP |
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        _this->_hWndParent,
        _this->_id,
        NULL,
        NULL);
    assert(hWnd != NULL);

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}
