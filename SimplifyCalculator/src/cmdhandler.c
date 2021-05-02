#include "platform.h"

#include "ribbon.h"

extern HWND _hFrameWnd;

static LONG _cRef = 1;

static HRESULT STDMETHODCALLTYPE QueryInterface(IUICommandHandler* This, REFIID riid, void** ppvObject)
{
    if (!ppvObject)
    {
        return E_POINTER;
    }

    if (!IsEqualIID(riid, &IID_IUICommandHandler)
        && !IsEqualIID(riid, &IID_IUnknown)) {
        *ppvObject = NULL;
        return(E_NOINTERFACE);
    }

    *ppvObject = This;

    This->lpVtbl->AddRef(This);
    return S_OK;

}

static ULONG STDMETHODCALLTYPE AddRef(IUICommandHandler* This)
{
    return(InterlockedIncrement(&_cRef));
}

static ULONG STDMETHODCALLTYPE Release(IUICommandHandler* This)
{
    LONG cRef = InterlockedDecrement(&_cRef);

    if (cRef == 0)
    {
        free(This);
    }

    return cRef;
}

static HRESULT STDMETHODCALLTYPE Execute(IUICommandHandler* This,
    UINT32 commandId,
    UI_EXECUTIONVERB verb,
    const PROPERTYKEY* key,
    const PROPVARIANT* currentValue,
    IUISimplePropertySet* commandExecutionProperties)
{
    if (_hFrameWnd)
    {
        PostMessage(_hFrameWnd, WM_RIBBON_COMMAND, commandId, 0);
    }

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE UpdateProperty(IUICommandHandler* This,
    UINT32 commandId,
    REFPROPERTYKEY key,
    const PROPVARIANT* currentValue,
    PROPVARIANT* newValue)
{
    return E_NOTIMPL;
}

IUICommandHandlerVtbl myCommand_Vtbl = {
    QueryInterface,
    AddRef,
    Release,
    Execute,
    UpdateProperty
};