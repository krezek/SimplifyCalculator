#include "platform.h"
#include "main_wnd.h"
#include "resource.h"

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Simplify Calculator");

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
    mw->_panels->_OnInitializeFunc(mw->_panels);
}

static void OnSize(MainWindow* mw)
{
}

static void OnPaint(MainWindow* mw)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(mw->_baseWindow._hWnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

    mw->_panels->_DrawListFunc(mw->_panels, hdc);

    EndPaint(mw->_baseWindow._hWnd, &ps);
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
        mw->_client_width = LOWORD(lParam);
        mw->_client_height = HIWORD(lParam);

        OnSize(mw);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(_this->_hWnd, uMsg, wParam, lParam);
    }
}
