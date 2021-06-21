#include "platform.h"

#include <text_edit.h>

static BOOL Create(BaseWindow* _this);

TextEdit* TextEdit_init()
{
    TextEdit* wnd = (TextEdit*)malloc(sizeof(TextEdit));
    assert(wnd != NULL);

    BaseWindow_default((BaseWindow*)wnd);

    //wnd->_baseWindow._HandleMessageFunc = HandleMessage;
    wnd->_baseWindow._CreateFunc = Create;

    return wnd;
}

void TextEdit_free(TextEdit* wnd)
{
    free(wnd);
}

static BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindowEx(
        0,
        WC_EDIT,
        (PCTSTR)NULL,
        SBARS_SIZEGRIP |
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_TABSTOP,
        0, 0, 0, 0,
        _this->_hWndParent,
        _this->_id,
        NULL,
        NULL);
    assert(hWnd != NULL);

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}
