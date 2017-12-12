#include "stdafx.h"
#include "Editor.h"

class VSCode : public Editor
{
	WCHAR szPath_[_MAX_PATH];
	bool installed_;

public:
	VSCode()
	{
		installed_ = false;
		HKEY key;
		LPCWSTR szVSCodeHive = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{EA457B21-F73E-494C-ACAB-524FDE069978}_is1";
		LONG lRet = RegOpenKeyW(HKEY_LOCAL_MACHINE, szVSCodeHive, &key);
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
		swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"code.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"-g \"%s\":%d", pszFilename, lineNumber );
	}
};

std::unique_ptr<Editor> DiscoverVSCode()
{
	return std::make_unique<VSCode>();
}


