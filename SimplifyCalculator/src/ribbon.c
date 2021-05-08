#include "platform.h"

#include "ribbon.h"
#include "ids.h"

DEFINE_GUID(IID_IUIFRAMEWORK, 0xf4f0385d, 0x6872, 0x43a8, 0xad, 0x09, 0x4c, 0x33, 0x9c, 0xb3, 0xf5, 0xc5);
DEFINE_GUID(IID_IUIAPPLICATION, 0xd428903c, 0x729a, 0x491d, 0x91, 0x0d, 0x68, 0x2a, 0x08, 0xff, 0x25, 0x22);
DEFINE_GUID(IID_IUIRIBBON, 0x803982ab, 0x370a, 0x4f7e, 0xa9, 0xe7, 0x87, 0x84, 0x03, 0x6a, 0x6e, 0x26);
DEFINE_GUID(IID_IUICommandHandler, 0xefc7bdf, 0x1fdc, 0x487a, 0xbc, 0x9b, 0x24, 0xcd, 0x19, 0x29, 0x1a, 0x17);
DEFINE_GUID(IID_IPropertyStore, 0x886d8eeb, 0x8cf2, 0x4446, 0x8d, 0x02, 0xcd, 0xba, 0x1d, 0xbd, 0xcf, 0x99);

DEFINE_GUID(IID_IUImage, 0x23c8c838, 0x4de6, 0x436b, 0xab, 0x01, 0x55, 0x54, 0xbb, 0x7c, 0x30, 0xdd);
DEFINE_GUID(IID_IUIImageFromBitmap, 0x18aba7f3, 0x4c1c, 0x4ba2, 0xbf, 0x6c, 0xf5, 0xc3, 0x32, 0x6f, 0xa8, 0x16);
DEFINE_GUID(IID_IUICollection, 0xdf4f45bf, 0x6f9d, 0x4dd7, 0x9d, 0x68, 0xd8, 0xf9, 0xcd, 0x18, 0xc4, 0xdb);
DEFINE_GUID(IID_IUISimplePropertySet, 0xc205bb48, 0x5b1c, 0x4219, 0xa1, 0x06, 0x15, 0xbd, 0x0a, 0x5f, 0x24, 0xe2);

IUIApplication* _pApplication = NULL;
IUIFramework* _pFramework = NULL;
IUICommandHandler* _pCommandHandler = NULL;
IUICommandHandler* _pFontCmdHandler = NULL;

LONG _cRef = 1;
UINT _uRibbonHeight = 0;

extern IUICommandHandlerVtbl myCommand_Vtbl;
extern IUICommandHandlerVtbl myFontCmd_Vtbl;

HWND _hFrameWnd;


HRESULT STDMETHODCALLTYPE QueryInterface(IUIApplication* This, REFIID vtblID, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (!IsEqualIID(vtblID, &IID_IUIAPPLICATION)
        && !IsEqualIID(vtblID, &IID_IUnknown)) {
        *ppv = NULL;
        return(E_NOINTERFACE);
    }

    *ppv = This;

    This->lpVtbl->AddRef(This);

    return S_OK;
}

ULONG STDMETHODCALLTYPE AddRef(IUIApplication* This)
{
    return(InterlockedIncrement(&_cRef));
}

ULONG STDMETHODCALLTYPE Release(IUIApplication* This)
{
    LONG cRef = InterlockedDecrement(&_cRef);
    if (cRef == 0)
    {
        free(This);
    }

    return cRef;
}

HRESULT STDMETHODCALLTYPE OnViewChanged(IUIApplication* This, UINT32 viewId,
    UI_VIEWTYPE typeID, IUnknown* view, UI_VIEWVERB verb, INT32 uReasonCode)
{
    HRESULT hr = E_NOTIMPL;

    if (UI_VIEWTYPE_RIBBON == typeID)
    {
        switch (verb)
        {
        case UI_VIEWVERB_CREATE:
            hr = S_OK;
            break;

        case UI_VIEWVERB_SIZE:
        {
            IUIRibbon* pRibbon = NULL;

            hr = view->lpVtbl->QueryInterface(view, &IID_IUIRIBBON, &pRibbon);
            if (SUCCEEDED(hr))
            {
                hr = pRibbon->lpVtbl->GetHeight(pRibbon, &_uRibbonHeight);
                pRibbon->lpVtbl->Release(pRibbon);

                if (_hFrameWnd)
                {
                    SendMessage(_hFrameWnd, WM_RIBBON_HEIGHT_CHANGED, _uRibbonHeight, 0);
                }
            }
        }
            break;

        case UI_VIEWVERB_DESTROY:
            hr = S_OK;
            break;
        }
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE OnCreateUICommand(IUIApplication* This, UINT32 commandId,
    UI_COMMANDTYPE typeID, IUICommandHandler** commandHandler)
{
    HRESULT hr = E_NOTIMPL;

    switch (commandId)
    {
    case cmdFontOnly:
    {
        if (_pFontCmdHandler == NULL)
        {
            _pFontCmdHandler = (IUICommandHandler*)malloc(sizeof(IUICommandHandler));
            if (!_pFontCmdHandler) {
                return E_NOTIMPL;
            }
            (IUICommandHandlerVtbl*)_pFontCmdHandler->lpVtbl = &myFontCmd_Vtbl;
        }

        hr = _pFontCmdHandler->lpVtbl->QueryInterface(_pFontCmdHandler, &IID_IUICommandHandler, commandHandler);
        break;
    }
    default:
    {
        if (_pCommandHandler == NULL)
        {
            _pCommandHandler = (IUICommandHandler*)malloc(sizeof(IUICommandHandler));
            if (!_pCommandHandler) {
                return E_NOTIMPL;
            }
            (IUICommandHandlerVtbl*)_pCommandHandler->lpVtbl = &myCommand_Vtbl;
        }

        hr = _pCommandHandler->lpVtbl->QueryInterface(_pCommandHandler, &IID_IUICommandHandler, commandHandler);
    }
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE OnDestroyUICommand(IUIApplication* This, UINT32 commandId,
    UI_COMMANDTYPE typeID, IUICommandHandler* commandHandler)
{
    return E_NOTIMPL;
}

IUIApplicationVtbl myRibbon_Vtbl = { 
    QueryInterface,
    AddRef,
    Release,
    OnViewChanged,
    OnCreateUICommand,
    OnDestroyUICommand 
};

int CreateRibbon(HWND hWnd)
{
    if (!hWnd)
    {
        return -1;
    }

    _hFrameWnd = hWnd;

    _pApplication = (IUIApplication*)malloc(sizeof(IUIApplication));
    if (!_pApplication) {
        return -1;
    }

    (IUIApplicationVtbl*)_pApplication->lpVtbl = &myRibbon_Vtbl;
    VOID* ppvObj = NULL;

    HRESULT hr = _pApplication->lpVtbl->QueryInterface(_pApplication, &IID_IUIAPPLICATION, &ppvObj);
    if (FAILED(hr))
    {
        return -1;
    }

    hr = CoCreateInstance(&CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, &IID_IUIFRAMEWORK, &_pFramework);
    if (FAILED(hr))
    {
        return -1;
    }

    hr = _pFramework->lpVtbl->Initialize(_pFramework, _hFrameWnd, (IUIApplication*)ppvObj);
    if (FAILED(hr))
    {
        _pApplication->lpVtbl->Release(_pApplication);
        return -1;
    }

    hr = _pFramework->lpVtbl->LoadUI(_pFramework, GetModuleHandle(NULL), L"APPLICATION_RIBBON");
    if (FAILED(hr))
    {
        _pApplication->lpVtbl->Release(_pApplication);
        return -1;
    }

    _pApplication->lpVtbl->Release(_pApplication);

    return 0;
}

void DestroyRibbon()
{
    if (_pFontCmdHandler)
    {
        _pFontCmdHandler->lpVtbl->Release(_pFontCmdHandler);
        _pFontCmdHandler = NULL;
    }

    if (_pCommandHandler)
    {
        _pCommandHandler->lpVtbl->Release(_pCommandHandler);
        _pCommandHandler = NULL;
    }

    if (_pFramework)
    {
        HRESULT hr = ((IUIFramework*)_pFramework)->lpVtbl->Destroy(_pFramework);
        ((IUIFramework*)_pFramework)->lpVtbl->Release(_pFramework);
        _pFramework = NULL;
    }

    if (_pApplication)
    {
        _pApplication->lpVtbl->Release(_pApplication);
        _pApplication = NULL;
    }
}