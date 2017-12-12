#include "stdafx.h"
#include "Editor.h"

class SlickEdit : public Editor
{
	WCHAR szPath_[_MAX_PATH];
	bool installed_;

public:
	SlickEdit()
	{
		installed_ = false;
		HKEY key;
		LPCWSTR szSlickEditHive = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\vs.exe";
		LONG lRet = RegOpenKeyW(HKEY_LOCAL_MACHINE, szSlickEditHive, &key);
		if( lRet == ERROR_SUCCESS ) 
		{
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(szPath_);
			lRet = RegQueryValueExW(key, L"Path", NULL, &dwType, (LPBYTE)szPath_, &dwSize);
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
		swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"vs.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"\"%s\" -#%d", pszFilename, lineNumber );
	}
};

std::unique_ptr<Editor> DiscoverSlickEdit()
{
	return std::make_unique<SlickEdit>();
}

