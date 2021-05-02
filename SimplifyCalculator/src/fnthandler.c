#include "platform.h"

#include "ribbon.h"

DEFINE_GUID(IID_IPropertyStore, 0x886d8eeb, 0x8cf2, 0x4446, 0x8d, 0x02, 0xcd, 0xba, 0x1d, 0xbd, 0xcf, 0x99);

extern HWND _hFrameWnd;

typedef struct {
    wchar_t _family[1024];
    double _size;
    unsigned int _bold;
    unsigned int _italic;
} _FontProp;

extern _FontProp _fontProp;

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
    HRESULT hr = E_NOTIMPL;
    if ((key) && (key->pid == UI_PKEY_FontProperties.pid))
    {
        // Font properties have changed.
        switch (verb)
        {
        case UI_EXECUTIONVERB_EXECUTE:
        {
            hr = E_POINTER;
            if (commandExecutionProperties != NULL)
            {
                // Get the changed properties.
                PROPVARIANT varChanges;
                hr = commandExecutionProperties->lpVtbl->GetValue(commandExecutionProperties,
                    &UI_PKEY_FontProperties_ChangedProperties, &varChanges);
                if (SUCCEEDED(hr))
                {
                    IPropertyStore* pChanges;
                    hr = varChanges.punkVal->lpVtbl->QueryInterface(varChanges.punkVal,
                        &IID_IPropertyStore, &pChanges);
                    if (SUCCEEDED(hr))
                    {
                        PROPVARIANT propvar;
                        PropVariantInit(&propvar);

                        hr = pChanges->lpVtbl->GetValue(pChanges, &UI_PKEY_FontProperties_Family, &propvar);

                        if (SUCCEEDED(hr))
                        {
                            // Get the string for the font family.
                            PWSTR pszFamily;
                            hr = PropVariantToStringAlloc(&propvar, &pszFamily);

                            if (SUCCEEDED(hr) && pszFamily)
                            {
                                wcscpy_s(_fontProp._family, sizeof(_fontProp._family) / sizeof(wchar_t), pszFamily);
                                CoTaskMemFree(pszFamily);
                            }
                            PropVariantClear(&propvar);
                        }

                        hr = pChanges->lpVtbl->GetValue(pChanges, &UI_PKEY_FontProperties_Size, &propvar);

                        if (SUCCEEDED(hr))
                        {
                            // Get the decimal font size value.
                            DECIMAL decSize;
                            if (propvar.vt == VT_DECIMAL)
                            {
                                decSize = propvar.decVal;
                            }
                            DOUBLE dSize;
                            VarR8FromDec(&decSize, &dSize);
                            // Zero is used as "Not Available" value.
                            if (dSize > 0)
                            {
                                _fontProp._size = dSize;
                            }
                            PropVariantClear(&propvar);
                        }

                        hr = pChanges->lpVtbl->GetValue(pChanges, &UI_PKEY_FontProperties_Bold, &propvar);

                        if (SUCCEEDED(hr))
                        {
                            UINT uValue;
                            hr = PropVariantToUInt32(&propvar, &uValue);
                            if (SUCCEEDED(hr))
                            {
                                _fontProp._bold = uValue;
                            }
                            PropVariantClear(&propvar);
                        }

                        hr = pChanges->lpVtbl->GetValue(pChanges, &UI_PKEY_FontProperties_Italic, &propvar);

                        if (SUCCEEDED(hr))
                        {
                            UINT uValue;
                            hr = PropVariantToUInt32(&propvar, &uValue);
                            if (SUCCEEDED(hr))
                            {
                                _fontProp._italic = uValue;
                            }
                            PropVariantClear(&propvar);
                        }

                        pChanges->lpVtbl->Release(pChanges);

                        PostMessage(_hFrameWnd, WM_RIBBON_FONT_CHANGED, 0, 0);
                    }
                    PropVariantClear(&varChanges);
                }
            }
            break;
        }
        }
    }

    return hr;
}

HRESULT InitPropVariantFromString(_In_ PCWSTR psz, _Out_ PROPVARIANT* ppropvar)
{
    HRESULT hr = psz != NULL ? S_OK : E_INVALIDARG; // Previous API behavior counter to the SAL requirement.
    if (SUCCEEDED(hr))
    {
        SIZE_T const byteCount = (SIZE_T)((wcslen(psz) + 1) * sizeof(*psz));
        V_UNION(ppropvar, pwszVal) = (PWSTR)(CoTaskMemAlloc(byteCount));
        hr = V_UNION(ppropvar, pwszVal) ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            memcpy_s(V_UNION(ppropvar, pwszVal), byteCount, psz, byteCount);
            V_VT(ppropvar) = VT_LPWSTR;
        }
    }
    if (FAILED(hr))
    {
        PropVariantInit(ppropvar);
    }
    return hr;
}

HRESULT UIInitPropertyFromDecimal(const DECIMAL* decValue, PROPVARIANT* pPropVar)
{
    pPropVar->decVal = *decValue;
    pPropVar->vt = VT_DECIMAL;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE UpdateProperty(IUICommandHandler* This,
    UINT32 commandId,
    REFPROPERTYKEY key,
    const PROPVARIANT* currentValue,
    PROPVARIANT* newValue)
{
    HRESULT hr = E_NOTIMPL;
    if (key && (key->pid == UI_PKEY_FontProperties.pid))
    {
        hr = E_POINTER;
        if (currentValue != NULL)
        {
            IPropertyStore* pValues;

            hr = currentValue->punkVal->lpVtbl->QueryInterface(currentValue->punkVal, &IID_IPropertyStore, &pValues);
            if (SUCCEEDED(hr))
            {
                PROPVARIANT propvar;
                PropVariantInit(&propvar);

                InitPropVariantFromString(_fontProp._family, &propvar);
                pValues->lpVtbl->SetValue(pValues, &UI_PKEY_FontProperties_Family, &propvar);
                PropVariantClear(&propvar);

                DECIMAL decSize;
                VarDecFromR8((DOUBLE)_fontProp._size, &decSize);
                UIInitPropertyFromDecimal(&decSize, &propvar);
                pValues->lpVtbl->SetValue(pValues, &UI_PKEY_FontProperties_Size, &propvar);
                PropVariantClear(&propvar);

                newValue->vt = VT_UNKNOWN;
                newValue->punkVal = (IUnknown*)pValues;
                if (pValues)
                {
                    pValues->lpVtbl->AddRef(pValues);
                }

                pValues->lpVtbl->Release(pValues);
            }
        }
    }

    return hr;
}

IUICommandHandlerVtbl myFontCmd_Vtbl = {
    QueryInterface,
    AddRef,
    Release,
    Execute,
    UpdateProperty
};