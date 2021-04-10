#include "platform.h"

#include "items.h"
#include "main_wnd.h"
#include "resource.h"

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Simplify Calculator");

static const int g_scrollbar_width = 20;
static int g_statusbar_height;

static const int g_font_size = 12;
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

static void CreateFonts(MainWindow* mw, int font_size)
{
    HDC hdc = GetDC(mw->_baseWindow._hWnd);

    // Create app fonts
    int lfHeight = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    g_fixed_font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Courier New");
    g_bold_font = CreateFont(lfHeight, 0, 0, 0, 700, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Courier New");
    g_math_font = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Cambria");

    // Get TextMetrics for fixed font
    SelectObject(hdc, g_fixed_font);
    GetTextMetrics(hdc, &g_tmFixed);
    ReleaseDC(mw->_baseWindow._hWnd, hdc);
}

static void OnCreate(MainWindow* mw)
{
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

    CreateFonts(mw, g_font_size);

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
    
    mw->_panels->_OnInitializeFunc(mw->_panels, 
        mw->_baseWindow._hWnd,
        mw->_client_width - g_scrollbar_width,
        mw->_client_height - g_statusbar_height);
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
    mw->_client_width = width;
    mw->_client_height = height;

    SendMessage(mw->_hWndStatusBar, WM_SIZE, 0, 0);
    
    MoveWindow(mw->_hWndVScrollBar,
        width - g_scrollbar_width,
        0,
        g_scrollbar_width,
        height - g_statusbar_height,
        TRUE);

    mw->_panels->_ParentSizeChangedFunc(mw->_panels, 
        mw->_client_width - g_scrollbar_width, 
        mw->_client_height - g_statusbar_height);

    mw->_panels->_selected_panel->_UpdateCaretPosFunc(mw->_panels->_selected_panel);

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

    mw->_panels->_ParentPosChangedFunc(mw->_panels);
    mw->_panels->_selected_panel->_UpdateCaretPosFunc(mw->_panels->_selected_panel);

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

static void OnFocus(MainWindow* mw)
{
    CreateCaret(mw->_baseWindow._hWnd, (HBITMAP)NULL, 2, g_tmFixed.tmHeight);
    ShowCaret(mw->_baseWindow._hWnd);

    mw->_panels->_selected_panel->_UpdateCaretPosFunc(mw->_panels->_selected_panel);
}

static void OnKillFocus(MainWindow* mw)
{
    DestroyCaret();
}

static void OnKeyDown(MainWindow* mw, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case VK_RETURN:
        if (GetKeyState(VK_SHIFT) < 0)
        {
            Panel* p = mw->_panels->_AddNewPanelFunc(mw->_panels);
            SendMessage(mw->_baseWindow._hWnd, WM_PANEL_SIZE_CHANGED, (WPARAM)NULL, (LPARAM)p);
            mw->_panels->_selected_panel->_UpdateCaretPosFunc(mw->_panels->_selected_panel);
            //ShowCaret(mw->_baseWindow._hWnd);

        }
        else
        {
            mw->_panels->_selected_panel->_OnChar_ReturnFunc(mw->_panels->_selected_panel);
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
        mw->_panels->_selected_panel->_OnKey_LeftArrowFunc(mw->_panels->_selected_panel);
        break;

    case VK_RIGHT:      // Right arrow
        mw->_panels->_selected_panel->_OnKey_RightArrowFunc(mw->_panels->_selected_panel);
        break;
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
        break;

    case 0x0D:          // Carriage return 
        break;

    case 0x1B:        // Escape 
    case 0x0A:        // Linefeed 
        MessageBeep((UINT)-1);
        break;

    default:
        mw->_panels->_selected_panel->_OnChar_DefaultFunc(mw->_panels->_selected_panel, (wchar_t) wParam);
        break;
    }

    ShowCaret(mw->_baseWindow._hWnd);
}

static void OnPanelRepaint(MainWindow* mw, Panel* p)
{
    RECT rc;
    rc.left = p->_x;
    rc.top = p->_y;
    rc.right = rc.left + p->_width;
    rc.bottom = rc.top + p->_height;

    InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);
}

static void OnPanelSizeChanged(MainWindow* mw, Panel* p)
{
    mw->_panels->_PanelSizeChangedFunc(mw->_panels, p);
    SetScrollbarInfo(mw);

    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = rc.left + mw->_client_width - g_scrollbar_width;
    rc.bottom = rc.top + mw->_client_height - g_statusbar_height;

    InvalidateRect(mw->_baseWindow._hWnd, &rc, TRUE);
}

static void OnCommand(MainWindow* mw, WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case ID_FILE_EXIT:
        PostMessage(mw->_baseWindow._hWnd, WM_CLOSE, 0, 0);
        break;
    }
}

static void OnPaint(MainWindow* mw)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(mw->_baseWindow._hWnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

    mw->_panels->_DrawListFunc(mw->_panels, hdc, &ps.rcPaint);

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

    case WM_SIZING:
        return mw->_baseWindow._OnSizingFunc(&mw->_baseWindow, (RECT*)lParam);

    case WM_SIZE:
        OnSize(mw, LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_VSCROLL:
        OnVScroll(mw, wParam);
        return 0;

    case WM_MOUSEWHEEL:
        OnMouseWheel(mw, wParam);
        return 0;

    case WM_SETFOCUS:
        OnFocus(mw);
        return 0;

    case WM_KILLFOCUS:
        OnKillFocus(mw);
        return 0;

    case WM_KEYDOWN:
        OnKeyDown(mw, wParam, lParam);
        return 0;

    case WM_CHAR:
        OnChar(mw, wParam, lParam);
        return 0;

    case WM_COMMAND:
        OnCommand(mw, wParam, lParam);
        return 0;

    case WM_PANEL_REPAINT:
        OnPanelRepaint(mw, (Panel*)lParam);
        return 0;

    case WM_PANEL_SIZE_CHANGED:
        OnPanelSizeChanged(mw, (Panel*)lParam);
        return 0;

    case WM_DESTROY:
        OnDestroy(mw);
        return 0;

    default:
        return DefWindowProc(_this->_hWnd, uMsg, wParam, lParam);
    }
}
