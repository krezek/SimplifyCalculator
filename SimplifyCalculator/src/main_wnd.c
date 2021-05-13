#include "platform.h"

#include "ribbon.h"
#include "items.h"
#include "main_wnd.h"
#include "resource.h"
#include "ids.h"

typedef struct {
    wchar_t _family[1024];
    double _size;
    unsigned int _bold;
    unsigned int _italic;
} _FontProp;

_FontProp _fontProp;

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Simplify Calculator");

static const int g_scrollbar_width = 20;
static int g_statusbar_height;

HFONT g_math_font;

static void AfterCreate(BaseWindow* _this);
static LRESULT HandleMessage(BaseWindow* _this, UINT uMsg, WPARAM wParam, LPARAM lParam);

ATOM MainWindow_RegisterClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = BaseWindow_Proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_POPUPMENU);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_64));

    return RegisterClassEx(&wcex);
}

BOOL Create(BaseWindow* _this)
{
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        _this
    );

    _this->_hWnd = hWnd;

    return _this->_hWnd ? TRUE : FALSE;
}

MainWindow* MainWindow_init()
{
    MainWindow* mw = (MainWindow*)malloc(sizeof(MainWindow));
    assert(mw != NULL);

    BaseWindow_default((BaseWindow*)mw);

    mw->_client_width = mw->_client_height = 0;
    mw->_ribbon_height = 0;
    mw->_x_current_pos = mw->_y_current_pos = 0;
    mw->_xMaxScroll = mw->_yMaxScroll = 0;

    mw->_baseWindow._HandleMessageFunc = HandleMessage;
    mw->_baseWindow._CreateFunc = Create;
    mw->_baseWindow._AfterCreateFunc = AfterCreate;

    mw->_panels = PanelList_init();

    return mw;
}

void MainWindow_free(MainWindow* mw)
{
    PanelList_free(mw->_panels);
    free(mw);
}

static void OnCreate(MainWindow* mw)
{
    // Fill FontProp
    wcscpy_s(_fontProp._family, sizeof(_fontProp._family) / sizeof(wchar_t), L"Cambria");
    _fontProp._size = 16;
    _fontProp._bold = 1;
    _fontProp._italic = 1;

    // Create Font
    HDC hdc = GetDC(mw->_baseWindow._hWnd);
    int lfHeight = -MulDiv((int)_fontProp._size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    g_math_font = CreateFont(lfHeight, 0, 0, 0, _fontProp._bold, 
        _fontProp._italic ? FALSE : TRUE, 0, 0, 0, 0, 0, 0, 0, _fontProp._family);
    ReleaseDC(mw->_baseWindow._hWnd, hdc);

    if (CreateRibbon(mw->_baseWindow._hWnd))
    {
        ShowError(L"OnCreate:CreateRibbon:Error");
    }

    mw->_hWndVScrollBar = CreateWindowEx(
        0,
        L"SCROLLBAR",
        (PTSTR)NULL,
        WS_CHILD | WS_VISIBLE | SBS_VERT,
        0,
        0,
        0,
        0,
        mw->_baseWindow._hWnd,
        (HMENU)NULL,
        (HINSTANCE)GetWindowLongPtr(mw->_baseWindow._hWnd, GWLP_HINSTANCE),
        (PVOID)NULL
    );
    assert(mw->_hWndVScrollBar != NULL);

    mw->_hWndHScrollBar = CreateWindowEx(
        0,
        L"SCROLLBAR",
        (PTSTR)NULL,
        WS_CHILD | WS_VISIBLE | SBS_HORZ,
        0,
        0,
        0,
        0,
        mw->_baseWindow._hWnd,
        (HMENU)NULL,
        (HINSTANCE)GetWindowLongPtr(mw->_baseWindow._hWnd, GWLP_HINSTANCE),
        (PVOID)NULL
    );
    assert(mw->_hWndHScrollBar != NULL);

    mw->_hWndCorner = CreateWindowEx(
        0,
        L"BUTTON",
        (PTSTR)NULL,
        WS_CHILD | WS_VISIBLE | WS_DISABLED,
        0,
        0,
        0,
        0,
        mw->_baseWindow._hWnd,
        (HMENU)NULL,
        (HINSTANCE)GetWindowLongPtr(mw->_baseWindow._hWnd, GWLP_HINSTANCE),
        (PVOID)NULL
    );
    assert(mw->_hWndCorner != NULL);

    mw->_hWndStatusBar = CreateWindowEx(
        0,
        STATUSCLASSNAME,
        (PCTSTR)NULL,
        SBARS_SIZEGRIP |
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        mw->_baseWindow._hWnd,
        (HMENU)IDC_STATUSBAR,
        (HINSTANCE)GetWindowLongPtr(mw->_baseWindow._hWnd, GWLP_HINSTANCE),
        NULL);
    assert(mw->_hWndStatusBar != NULL);

    FontHandle fh;
    fh._hfont = g_math_font;
    Graphics_fontList_init(fh);

    RECT rc;
    GetClientRect(mw->_baseWindow._hWnd, &rc);

    mw->_client_width = rc.right - rc.left;
    mw->_client_height = rc.bottom - rc.top;

    SendMessage(mw->_hWndStatusBar, WM_SIZE, 0, 0);
    GetWindowRect(mw->_hWndStatusBar, &rc);
    g_statusbar_height = rc.bottom - rc.top; 
}

void SetScrollbarInfo(MainWindow* mw)
{
    SCROLLINFO siv, sih;

    int v1 = mw->_y_current_pos;
    int v2 = mw->_panels->_GetViewportHeightFunc(mw->_panels) - 
        mw->_client_height - v1 + mw->_ribbon_height + g_statusbar_height + g_scrollbar_width;
    int v = v1 + (v2 > 0 ? v2 : 0);

    mw->_yMaxScroll = v;
    siv.cbSize = sizeof(siv);
    siv.fMask = SIF_RANGE | SIF_POS;
    siv.nMin = 0;
    siv.nMax = mw->_yMaxScroll;
    siv.nPos = mw->_y_current_pos;
    SetScrollInfo(mw->_hWndVScrollBar, SB_CTL, &siv, TRUE);

    int h1 = mw->_x_current_pos;
    int h2 = mw->_panels->_GetViewportWidthFunc(mw->_panels) -
        mw->_client_width - h1 + g_scrollbar_width;
    int h = h1 + (h2 > 0 ? h2 : 0);

    mw->_xMaxScroll = h;
    sih.cbSize = sizeof(sih);
    sih.fMask = SIF_RANGE | SIF_POS;
    sih.nMin = 0;
    sih.nMax = mw->_xMaxScroll;
    sih.nPos = mw->_x_current_pos;
    SetScrollInfo(mw->_hWndHScrollBar, SB_CTL, &sih, TRUE);
}

static void WindowPropertyChanged(MainWindow* mw)
{
    SendMessage(mw->_hWndStatusBar, WM_SIZE, 0, 0);

    MoveWindow(mw->_hWndVScrollBar,
        mw->_client_width - g_scrollbar_width,
        mw->_ribbon_height,
        g_scrollbar_width,
        mw->_client_height - g_statusbar_height - mw->_ribbon_height - g_scrollbar_width,
        TRUE);
    InvalidateRect(mw->_hWndVScrollBar, NULL, TRUE);

    MoveWindow(mw->_hWndHScrollBar,
        0,
        mw->_client_height - g_statusbar_height - g_scrollbar_width,
        mw->_client_width - g_scrollbar_width,
        g_scrollbar_width,
        TRUE);
    InvalidateRect(mw->_hWndHScrollBar, NULL, TRUE);

    MoveWindow(mw->_hWndCorner,
        mw->_client_width - g_scrollbar_width,
        mw->_client_height - g_statusbar_height - g_scrollbar_width,
        g_scrollbar_width,
        g_scrollbar_width,
        TRUE);
    InvalidateRect(mw->_hWndCorner, NULL, TRUE);
        
    SetScrollbarInfo(mw);
}

static void OnSize(MainWindow* mw, int width, int height)
{
    if (!IsWindowVisible(mw->_baseWindow._hWnd))
        return;

    mw->_client_width = width;
    mw->_client_height = height;

    mw->_panels->_width = mw->_client_width;
    mw->_panels->_height = mw->_client_height;

    WindowPropertyChanged(mw);
    InvalidateRect(mw->_baseWindow._hWnd, NULL, TRUE);
}

void OnRibbonHeightChanged(MainWindow* mw, int height)
{
    mw->_ribbon_height = height;
    
    if (!IsWindowVisible(mw->_baseWindow._hWnd))
        return;

    WindowPropertyChanged(mw);

    mw->_panels->_y0 = mw->_ribbon_height;
    mw->_panels->_OnPosChangedFunc(mw->_panels);

    mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

    InvalidateRect(mw->_baseWindow._hWnd, NULL, TRUE);
}

void AfterCreate(BaseWindow* _this)
{
    MainWindow* mw = (MainWindow*)_this;

    mw->_panels->_x0 = 0;
    mw->_panels->_y0 = mw->_ribbon_height;
    mw->_panels->_width = mw->_client_width;
    mw->_panels->_height = mw->_client_height;

    mw->_panels->_OnInitFunc(mw->_panels, mw->_baseWindow._hWnd);
    
    mw->_panels->_AddNewPanelFunc(mw->_panels);
}

void OnFontChanged(MainWindow* mw)
{
    Graphics_fontList_free();
    DeleteObject(g_math_font);

    HDC hdc = GetDC(mw->_baseWindow._hWnd);
    int lfHeight = -MulDiv((int)_fontProp._size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    g_math_font = CreateFont(lfHeight, 0, 0, 0, _fontProp._bold,
        _fontProp._italic ? FALSE : TRUE, 0, 0, 0, 0, 0, 0, 0, _fontProp._family);
    ReleaseDC(mw->_baseWindow._hWnd, hdc);

    FontHandle fh;
    fh._hfont = g_math_font;
    Graphics_fontList_init(fh);

    mw->_panels->_OnFontChangedFunc(mw->_panels);
    WindowPropertyChanged(mw);

    mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

    InvalidateRect(mw->_baseWindow._hWnd, NULL, TRUE);
}

static void OnVScroll(MainWindow* mw, WPARAM wParam)
{
    int xDelta = 0;
    int yNewPos;
    int yDelta;

    switch (LOWORD(wParam))
    {
    case SB_PAGEUP:
        yNewPos = mw->_y_current_pos - 60;
        break;

    case SB_PAGEDOWN:
        yNewPos = mw->_y_current_pos + 60;
        break;

    case SB_LINEUP:
        yNewPos = mw->_y_current_pos - 20;
        break;

    case SB_LINEDOWN:
        yNewPos = mw->_y_current_pos + 20;
        break;

    case SB_THUMBPOSITION:
        yNewPos = HIWORD(wParam);
        break;

    default:
        yNewPos = mw->_y_current_pos;
    }

    yNewPos = max(0, yNewPos);
    yNewPos = min(mw->_yMaxScroll, yNewPos);

    // If the current position does not change, do not scroll.
    if (yNewPos == mw->_y_current_pos)
        return;

    // Determine the amount scrolled (in pixels). 
    yDelta = yNewPos - mw->_y_current_pos;

    // Reset the current scroll position. 
    mw->_panels->_y_current_pos = mw->_y_current_pos = yNewPos;

    mw->_panels->_OnPosChangedFunc(mw->_panels);
    mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

    RECT rc;
    rc.left = 0;
    rc.top = mw->_ribbon_height;
    rc.right = mw->_client_width - g_scrollbar_width;
    rc.bottom = mw->_client_height - g_statusbar_height - g_scrollbar_width;

    ScrollWindowEx(mw->_baseWindow._hWnd, -xDelta, -yDelta, &rc,
        &rc, (HRGN)NULL, (RECT*)NULL,
        SW_INVALIDATE);
    UpdateWindow(mw->_baseWindow._hWnd);

    // Reset the scroll bar. 
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = mw->_y_current_pos;
    SetScrollInfo(mw->_hWndVScrollBar, SB_CTL, &si, TRUE);
}

static void OnHScroll(MainWindow* mw, WPARAM wParam)
{
    int xDelta;
    int xNewPos;
    int yDelta = 0;

    switch (LOWORD(wParam))
    {
    case SB_PAGEUP:
        xNewPos = mw->_x_current_pos - 60;
        break;

    case SB_PAGEDOWN:
        xNewPos = mw->_x_current_pos + 60;
        break;

    case SB_LINEUP:
        xNewPos = mw->_x_current_pos - 20;
        break;

    case SB_LINEDOWN:
        xNewPos = mw->_x_current_pos + 20;
        break;

    case SB_THUMBPOSITION:
        xNewPos = HIWORD(wParam);
        break;

    default:
        xNewPos = mw->_x_current_pos;
    }

    xNewPos = max(0, xNewPos);
    xNewPos = min(mw->_xMaxScroll, xNewPos);

    // If the current position does not change, do not scroll.
    if (xNewPos == mw->_x_current_pos)
        return;

    // Determine the amount scrolled (in pixels). 
    xDelta = xNewPos - mw->_x_current_pos;

    // Reset the current scroll position. 
    mw->_panels->_x_current_pos = mw->_x_current_pos = xNewPos;

    mw->_panels->_OnPosChangedFunc(mw->_panels);
    mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

    RECT rc;
    rc.left = 0;
    rc.top = mw->_ribbon_height;
    rc.right = mw->_client_width - g_scrollbar_width;
    rc.bottom = mw->_client_height - g_statusbar_height - g_scrollbar_width;

    ScrollWindowEx(mw->_baseWindow._hWnd, -xDelta, -yDelta, &rc,
        &rc, (HRGN)NULL, (RECT*)NULL,
        SW_INVALIDATE);
    UpdateWindow(mw->_baseWindow._hWnd);

    // Reset the scroll bar. 
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = mw->_x_current_pos;
    SetScrollInfo(mw->_hWndHScrollBar, SB_CTL, &si, TRUE);
}

static void OnMouseWheel(MainWindow* mw, WPARAM wParam)
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    if (zDelta < 0)
        PostMessage(mw->_baseWindow._hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
    else
        PostMessage(mw->_baseWindow._hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
}

static void OnSetFocus(MainWindow* mw)
{
    mw->_panels->_selected_panel->_OnSetFocusFunc(mw->_panels->_selected_panel);
}

static void OnKillFocus(MainWindow* mw)
{
    mw->_panels->_selected_panel->_OnKillFocusFunc(mw->_panels->_selected_panel);
}

static void OnKeyDown(MainWindow* mw, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case VK_RETURN:
        if (GetKeyState(VK_SHIFT) < 0)
        {
            // Remove old selection
            (*mw->_panels->_selected_panel->_editor->_current_node->_pItem)->_setFocusFunc(
                *mw->_panels->_selected_panel->_editor->_current_node->_pItem, 0);

            // Adding new panel
            Panel* p = mw->_panels->_AddNewPanelFunc(mw->_panels);

            SetScrollbarInfo(mw);

            mw->_panels->_OnPosChangedFunc(mw->_panels);
            InvalidateRect(mw->_baseWindow._hWnd, NULL, TRUE);

            // update caret position
            mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

        }
        else
        {
            mw->_panels->_selected_panel->_OnChar_ReturnFunc(mw->_panels->_selected_panel);

            mw->_panels->_OnSelectedPanelChanged(mw->_panels);
            WindowPropertyChanged(mw);

            RECT rc;
            rc.left = mw->_panels->_selected_panel->_x0;
            rc.top = mw->_panels->_selected_panel->_y0;
            rc.right = rc.left + mw->_client_width;
            rc.bottom = rc.top + mw->_client_height;

            InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);

            mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);
        }
        
        break;
    
    case VK_HOME:       // Home 
        break;

    case VK_END:        // End 
        break;

    case VK_PRIOR:      // Page Up 
        break;

    case VK_NEXT:       // Page Down 
        break;

    case VK_LEFT:       // Left arrow 
    {
        RECT rc = mw->_panels->_selected_panel->_GetRectFunc(mw->_panels->_selected_panel);
        mw->_panels->_selected_panel->_OnKey_LeftArrowFunc(mw->_panels->_selected_panel);
        InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);
        break;
    }

    case VK_RIGHT:      // Right arrow
    {
        RECT rc = mw->_panels->_selected_panel->_GetRectFunc(mw->_panels->_selected_panel);
        mw->_panels->_selected_panel->_OnKey_RightArrowFunc(mw->_panels->_selected_panel);
        InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);
        break;
    }
    case VK_UP:         // Up arrow 
        break;

    case VK_DOWN:       // Down arrow 
        break;

    case VK_DELETE:     // Delete 
        break;
    }
}

static void OnChar(MainWindow* mw, WPARAM wParam, LPARAM lParam)
{
    HideCaret(mw->_baseWindow._hWnd);

    switch (wParam)
    {
    case 0x08:          // Backspace 
        mw->_panels->_selected_panel->_OnChar_BackspaceFunc(mw->_panels->_selected_panel);
        break;

    case 0x09:          // Tab 
    {
        // Todo: Implement Tab navigation
        if (GetKeyState(VK_SHIFT) < 0)
        {
        }
        else
        {
        }
    }
        break;

    case 0x0D:          // Carriage return 
        break;

    case 0x1B:        // Escape 
    case 0x0A:        // Linefeed 
        MessageBeep((UINT)-1);
        break;

    default:
    {
        mw->_panels->_selected_panel->_OnChar_DefaultFunc(mw->_panels->_selected_panel, (wchar_t)wParam);

        mw->_panels->_OnSelectedPanelChanged(mw->_panels);
        WindowPropertyChanged(mw);

        RECT rc;
        rc.left = mw->_panels->_selected_panel->_x0;
        rc.top = mw->_panels->_selected_panel->_y0;
        rc.right = rc.left + mw->_client_width;
        rc.bottom = rc.top + mw->_client_height;

        InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);

        mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);
        break;
    }
    }

    ShowCaret(mw->_baseWindow._hWnd);
}

static void OnRibbonCmd(MainWindow* mw, int cmd)
{
    switch (cmd)

    {
    case IDC_CMD_EXIT:
        PostMessage(mw->_baseWindow._hWnd, WM_CLOSE, 0, 0);
        return;

    default:
    {
        mw->_panels->_selected_panel->_OnCmdFunc(mw->_panels->_selected_panel, cmd);

        mw->_panels->_OnSelectedPanelChanged(mw->_panels);
        WindowPropertyChanged(mw);

        RECT rc;
        rc.left = mw->_panels->_selected_panel->_x0;
        rc.top = mw->_panels->_selected_panel->_y0;
        rc.right = rc.left + mw->_client_width;
        rc.bottom = rc.top + mw->_client_height;

        InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);

        mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);
    }
        break;
    }
}

static void OnMousLButtonDown(MainWindow* mw, int x, int y)
{
    Panel* p = mw->_panels->_GetPanelFromPointFunc(mw->_panels, x, y);
    if (p)
    {
        RECT rc1 = mw->_panels->_selected_panel->_GetRectFunc(mw->_panels->_selected_panel);
        (*mw->_panels->_selected_panel->_editor->_current_node->_pItem)->_setFocusFunc(
            *mw->_panels->_selected_panel->_editor->_current_node->_pItem, 0);
        InvalidateRect(mw->_baseWindow._hWnd, &rc1, TRUE);

        mw->_panels->_selected_panel = p;
        mw->_panels->_selected_panel->_editor->_OnUpdateCaret(mw->_panels->_selected_panel->_editor);

        rc1 = mw->_panels->_selected_panel->_GetRectFunc(mw->_panels->_selected_panel);
        (*mw->_panels->_selected_panel->_editor->_current_node->_pItem)->_setFocusFunc(
            *mw->_panels->_selected_panel->_editor->_current_node->_pItem, 1);
        InvalidateRect(mw->_baseWindow._hWnd, &rc1, TRUE);
    }
}

static void OnPaint(MainWindow* mw)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(mw->_baseWindow._hWnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

    mw->_panels->_OnPaintFunc(mw->_panels, hdc, &ps.rcPaint);
    
    EndPaint(mw->_baseWindow._hWnd, &ps);
}

static void OnDestroy(MainWindow* mw)
{
    Graphics_fontList_free();

    DeleteObject(g_math_font);

    DestroyRibbon();

    PostQuitMessage(0);
}

static LRESULT HandleMessage(BaseWindow* _this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow* mw = (MainWindow*)_this;

    switch (uMsg)
    {
    case WM_CREATE:
        OnCreate(mw);
        return 0;

    case WM_PAINT:
        OnPaint(mw);
        return 0;

    case WM_SIZING:
        return mw->_baseWindow._OnSizingFunc(&mw->_baseWindow, (RECT*)lParam);

    case WM_SIZE:
        OnSize(mw, LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_VSCROLL:
        OnVScroll(mw, wParam);
        return 0;

    case WM_HSCROLL:
        OnHScroll(mw, wParam);
        return 0;

    case WM_MOUSEWHEEL:
        OnMouseWheel(mw, wParam);
        return 0;

    case WM_LBUTTONDOWN:
        OnMousLButtonDown(mw, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_SETFOCUS:
        OnSetFocus(mw);
        return 0;

    case WM_KILLFOCUS:
        OnKillFocus(mw);
        return 0;

    case WM_KEYDOWN:
        OnKeyDown(mw, wParam, lParam);
        return 0;

    case WM_CHAR:
    {
        static int x = 2;
        if (wParam == L'^')
        {
            --x;
            if (x) return 0;
            x = 2;
        }

        OnChar(mw, wParam, lParam);
        return 0;
    }
    
    case WM_RIBBON_HEIGHT_CHANGED:
        OnRibbonHeightChanged(mw, (int)wParam);
        return 0;

    case WM_RIBBON_FONT_CHANGED:
        OnFontChanged(mw);
        return 0;
    case WM_RIBBON_COMMAND:
        OnRibbonCmd(mw, (int)wParam);
        return 0;

    case WM_DESTROY:
        OnDestroy(mw);
        return 0;

    default:
        return DefWindowProc(_this->_hWnd, uMsg, wParam, lParam);
    }
}
