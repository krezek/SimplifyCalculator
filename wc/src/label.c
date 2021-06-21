#include "platform.h"

#include <label.h>

static BOOL Create(BaseWindow* _this);

Label* Label_init()
{
    Label* wnd = (Label*)malloc(sizeof(Label));
    assert(wnd != NULL);

    BaseWindow_default((BaseWindow*)wnd);

    //wnd->_baseWindow._HandleMessageFunc = HandleMessage;
    wnd->_baseWindow._CreateFunc = Create;

    return wnd;
}

void Label_free(Label* wnd)
{
    free(wnd);
}

static BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindowEx(
        0,
        WC_STATIC,
        (PCTSTR)NULL,
        SBARS_SIZEGRIP |
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS,
        0, 0, 0, 0,
        _this->_hWndParent,
        _this->_id,
        NULL,
        NULL);
    assert(hWnd != NULL);

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}
