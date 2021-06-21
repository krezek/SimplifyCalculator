#include "platform.h"

#include <list_view.h>

static BOOL Create(BaseWindow* _this);

ListView* ListView_init()
{
    ListView* wnd = (ListView*)malloc(sizeof(ListView));
    assert(wnd != NULL);

    BaseWindow_default((BaseWindow*)wnd);

    //wnd->_baseWindow._HandleMessageFunc = HandleMessage;
    wnd->_baseWindow._CreateFunc = Create;

    return wnd;
}

void ListView_free(ListView* wnd)
{
    free(wnd);
}

static BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindowEx(0,
        WC_LISTVIEW,
        (PCTSTR)NULL,
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_BORDER | WS_CLIPSIBLINGS,
        0, 0, 0, 0,
        _this->_hWndParent,
        _this->_id,
        NULL,
        NULL);
    assert(hWnd != NULL);

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}
