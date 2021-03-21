#include "platform.h"
#include "main_wnd.h"
#include "resource.h"

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Simplify Calculator");

static const int g_scrollbar_width = 20;
static int g_statusbar_height;

HFONT g_bold_font, g_math_font, g_fixed_font;
TEXTMETRIC g_tmFixed;

static LRESULT HandleMessage(BaseWindow* _this, UINT uMsg, WPARAM wParam, LPARAM lParam);

ATOM MainWindow_RegisterClass()
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
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
        WS_OVERLAPPEDWINDOW,
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
    mw->_x_current_pos = mw->_y_current_pos = 0;
    mw->_yMaxScroll = 0;

    mw->_baseWindow._HandleMessageFunc = HandleMessage;
    mw->_baseWindow._CreateFunc = Create;

    mw->_panels = PanelLinkedList_init();

    return mw;
}

void MainWindow_free(MainWindow* mw)
{
    PanelLinkedList_free(mw->_panels);
    free(mw);
}

static void OnCreate(MainWindow* mw)
{
    // Create app fonts
    HDC hdc = GetDC(mw->_baseWindow._hWnd);
    int lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(mw->_baseWindow._hWnd, hdc);

    g_fixed_font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Courier New");
    g_bold_font = CreateFont(lfHeight, 0, 0, 0, 700, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Courier New");
    g_math_font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Cambria");

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

    mw->_panels->_hWndParent = mw->_baseWindow._hWnd;
    mw->_panels->_OnInitializeFunc(mw->_panels);
}

void SetScrollbarInfo(MainWindow* mw)
{
    SCROLLINFO si;

    int v1 = mw->_y_current_pos;
    int v2 = mw->_panels->_GetViewportHeightFunc(mw->_panels) - mw->_client_height - v1 + g_statusbar_height;
    int v = v1 + (v2 > 0 ? v2 : 0);

    mw->_yMaxScroll = v;
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = mw->_yMaxScroll;
    si.nPos = mw->_y_current_pos;
    SetScrollInfo(mw->_hWndVScrollBar, SB_CTL, &si, TRUE);
}

static void OnSize(MainWindow* mw, int width, int height)
{
    RECT statusBarRect;

    mw->_client_width = width;
    mw->_client_height = height;

    SendMessage(mw->_hWndStatusBar, WM_SIZE, 0, 0);
    GetWindowRect(mw->_hWndStatusBar, &statusBarRect);
    g_statusbar_height = statusBarRect.bottom - statusBarRect.top;

    MoveWindow(mw->_hWndVScrollBar,
        width - g_scrollbar_width,
        0,
        g_scrollbar_width,
        height - g_statusbar_height,
        TRUE);

    mw->_panels->_client_width = width - g_scrollbar_width;
    mw->_panels->_client_height = height;
    mw->_panels->_ParentSizeChangedFunc(mw->_panels);

    SetScrollbarInfo(mw);
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

    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = mw->_client_width - g_scrollbar_width;
    rc.bottom = mw->_client_height - g_statusbar_height;

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

static void OnMouseWheel(MainWindow* mw, WPARAM wParam)
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    if (zDelta < 0)
        PostMessage(mw->_baseWindow._hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
    else
        PostMessage(mw->_baseWindow._hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
}

static void OnPaint(MainWindow* mw)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(mw->_baseWindow._hWnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

    mw->_panels->_DrawListFunc(mw->_panels, hdc);

    EndPaint(mw->_baseWindow._hWnd, &ps);
}

static void OnDestroy(MainWindow* mw)
{
    DeleteObject(g_bold_font);
    DeleteObject(g_fixed_font);
    DeleteObject(g_math_font);

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

    case WM_SIZE:
        OnSize(mw, LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_VSCROLL:
        OnVScroll(mw, wParam);
        return 0;

    case WM_MOUSEWHEEL:
        OnMouseWheel(mw, wParam);
        return 0;

    case WM_DESTROY:
        OnDestroy(mw);
        return 0;

    default:
        return DefWindowProc(_this->_hWnd, uMsg, wParam, lParam);
    }
}
