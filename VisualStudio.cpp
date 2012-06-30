#include "stdafx.h"
#include "Editor.h"
#include "ZeeException.h"

#pragma warning(disable:4278)

#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" \
    raw_interfaces_only  \
    named_guids

extern bool g_ForceSetup;

class VisualStudio : public Editor
{
    WCHAR szExePath_[_MAX_PATH];
    bool installed_;

    void TryVSVersion(LPCWSTR szVSHive)
    {
        HKEY key;

        LONG lRet = RegOpenKeyW(HKEY_LOCAL_MACHINE, szVSHive, &key);
        if( lRet == ERROR_SUCCESS ) 
        {
            DWORD dwType = REG_SZ;
            WCHAR szPath[_MAX_PATH];
            DWORD dwSize = sizeof(szPath);
            lRet = RegQueryValueExW(key, L"InstallDir", NULL, &dwType, (LPBYTE)szPath, &dwSize);
            if( lRet == ERROR_SUCCESS )
            {
                swprintf_s(szExePath_, arraylen(szExePath_), L"%s%s", szPath, L"devenv.exe" );
                installed_ = true;
            }
        }
    }

public:
    VisualStudio()
    {
        installed_ = false;

        TryVSVersion(L"SOFTWARE\\Microsoft\\VisualStudio\\8.0");
        if( !installed_ )
        {
            TryVSVersion(L"SOFTWARE\\Microsoft\\VisualStudio\\9.0");
            if( !installed_ )
            {
                TryVSVersion(L"SOFTWARE\\Microsoft\\VisualStudio\\10.0");
                if( !installed_ )
                {
                    // get out
                    return;
                }
            }
        }

        CoInitialize(NULL);
    }

    ~VisualStudio()
    {
        CoUninitialize();
    }

    virtual bool IsInstalled()
    {
        return installed_;
    }
private:
    virtual void GetExecutable(LPWSTR pszExe, int exeSize)
    {
        _ASSERT(FALSE);
    }

    virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
    {
        _ASSERT(FALSE);
    }

    virtual void OpenFile(LPCWSTR fileName, int lineNumber )
    {
        if( g_ForceSetup )
        {
            // Do nothing, se we're in setup mode. Ugly.
            return;
        }

        WCHAR szFullPath[_MAX_PATH];
        _wfullpath(szFullPath, fileName, _MAX_PATH);

        if( !CreateFileIfNeeded(szFullPath) )
        {
            return;
        }

        EnvDTE::_DTE* pDTE = this->GetActiveVSProcess();
        if (pDTE == NULL)
        {
            // Need to start a new instance
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory( &si, sizeof(si) );
            si.cb = sizeof(si);
            ZeroMemory( &pi, sizeof(pi) );

            if( !CreateProcess( NULL,
                szExePath_,		// Command line
                NULL,			// Process handle not inheritable
                NULL,			// Thread handle not inheritable
                FALSE,			// Set handle inheritance to FALSE
                NORMAL_PRIORITY_CLASS, // Creation flags
                NULL,			// Use parent's environment block
                NULL,			// Use parent's starting directory 
                &si,			// Pointer to STARTUPINFO structure
                &pi )			// Pointer to PROCESS_INFORMATION structure
            ) 
            {
                ZeeException exception(L"a", L"Cannot start Visual Studio" );
                throw exception;
            }

            // Give it some time to start and try getting DTE interface
            int retries = 10;
            while(true)
            {
                Sleep(500);
                pDTE = this->GetActiveVSProcess();
                if( pDTE != NULL )
                {
                    break;
                }
                if( retries-- == 0 )
                {
                    ZeeException exception(L"a", L"Visual Studio timed out, try again" );
                    throw exception;
                }
            }
        }

        _ASSERT(pDTE != NULL);

        BSTR bstrFileName = SysAllocString(szFullPath);
        BSTR bstrViewKind = SysAllocString(L"{7651A703-06E5-11D1-8EBD-00A0C90F26EA}"); // vsViewKindTextView

        HRESULT hr = S_OK;
        EnvDTE::Window* pWindow;

        if (SUCCEEDED(hr = pDTE->OpenFile(bstrViewKind, bstrFileName, &pWindow)))
        {
            HWND hwnd;
            EnvDTE::Window* pMainWindow;
            pDTE->get_MainWindow(&pMainWindow);

            pMainWindow->Activate();

            pMainWindow->get_HWnd((long*)&hwnd);
            SetForegroundWindow(hwnd);

            pWindow->Activate();

            if (lineNumber > 0)
            {
                EnvDTE::Document* pDoc;
                pDTE->get_ActiveDocument(&pDoc);
                if (pDoc)
                {
                    IDispatch* pDispSelection;
                    pDoc->get_Selection(&pDispSelection);

                    EnvDTE::TextSelection* pSelection;
                    pDispSelection->QueryInterface(EnvDTE::IID_TextSelection, (void **)&pSelection);

                    if (pSelection)
                    {
                        pSelection->GotoLine(lineNumber, VARIANT_FALSE);
                    }
                    pSelection->Release();
                }
            }
        }
        else
        {
            SysFreeString(bstrFileName);
            SysFreeString(bstrViewKind);
            pDTE->Release();
            ZeeException exception(L"a", L"Visual Studio failed to open file '%s'", fileName );
            throw exception;
        }
        SysFreeString(bstrFileName);
        SysFreeString(bstrViewKind);
        pDTE->Release();
    }

    EnvDTE::_DTE* GetActiveVSProcess()
    {
        HRESULT hr = S_OK;

        EnvDTE::_DTE *pDTE = NULL;

        IUnknown* pUnk = NULL;

        CLSID clsidDTE = GUID_NULL;
        CLSIDFromProgID(L"VisualStudio.DTE", &clsidDTE);

        if( clsidDTE != GUID_NULL )
        {
            if (SUCCEEDED(hr = GetActiveObject(clsidDTE, NULL, &pUnk)))
            {
                pUnk->QueryInterface(&pDTE);
            }
        }
        return pDTE;
    }
};

Editor* DiscoverVisualStudio()
{
    return new VisualStudio();
}


