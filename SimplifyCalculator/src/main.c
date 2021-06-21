#include "platform.h"
#include "main_wnd.h"


#ifdef _DEBUG
#define SHOWCONSOLE
void RedirectIOToConsole();
#endif

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef SHOWCONSOLE
    RedirectIOToConsole();
#endif

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        ShowError(L"WinMain:CoInitialize");
        return -1;
    }

    if (!MainWindow_RegisterClass())
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Formula Renderer"),
            0);

        return -1;
    }

    BaseWindow* mainWindow = (BaseWindow*)MainWindow_init();

    if (!mainWindow->_CreateFunc(mainWindow))
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Formula Renderer"),
            0);

        return -1;
    }

    mainWindow->_ShowFunc(mainWindow, nCmdShow);
    mainWindow->_UpdateFunc(mainWindow);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    MainWindow_free((MainWindow*)mainWindow);

    CoUninitialize();

    return (int)msg.wParam;
}

void RedirectIOToConsole()
{
    int hConHandle;
    HANDLE hStdHandle;
    FILE* fp;

    AllocConsole();

    hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)hStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    freopen_s(&fp, "CONOUT$", "w", stdout);

    hStdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)hStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "r");
    freopen_s(&fp, "CONIN$", "r", stdin);

    hStdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)hStdHandle, _O_TEXT);
    fp = _fdopen(hConHandle, "w");
    freopen_s(&fp, "CONOUT$", "w", stderr);
}
