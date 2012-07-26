#include "stdafx.h"
#include "Editor.h"

class Sublime : public Editor
{
    WCHAR szPath_[_MAX_PATH];
    bool installed_;

public:
    Sublime()
    {
        installed_ = false;
        HKEY key;
        REGSAM samDesired = KEY_READ | KEY_WOW64_64KEY;
        LPCWSTR szSublimeTextHive = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Sublime Text 2_is1";
                                     //"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Sublime Text 2_is1";
        LONG lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, szSublimeTextHive, 0, samDesired, &key);
        if( lRet == ERROR_SUCCESS ) 
        {
            DWORD dwType = REG_SZ;
            DWORD dwSize = sizeof(szPath_);
            lRet = RegQueryValueExW(key, L"InstallLocation", NULL, &dwType, (LPBYTE)szPath_, &dwSize);
            if( lRet == ERROR_SUCCESS ) installed_ = true;
        }
    }
    virtual bool IsInstalled()
    {
        return installed_;
    }
private:
    virtual void GetExecutable(LPWSTR pszExe, int exeSize)
    {
        auto len = wcslen(szPath_);
        if(szPath_[len-1]==L'\\')
            szPath_[len-1]='\0';
        swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"sublime_text.exe" );
    }

    virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
    {
        swprintf_s(pszParams, paramSize, L"\"%s\":%d:%d", pszFilename, lineNumber, 1 );
    }
};

Editor* DiscoverSublime()
{
    return new Sublime();
}


