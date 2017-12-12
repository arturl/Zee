#include "stdafx.h"
#include "Editor.h"

class UltraEdit : public Editor
{
	WCHAR szPath_[_MAX_PATH];
	bool installed_;

public:
	UltraEdit()
	{
		installed_ = false;
		HKEY key;
		LPCWSTR szUltraEditHive = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\uedit32.exe";
		LONG lRet = RegOpenKeyW(HKEY_LOCAL_MACHINE, szUltraEditHive, &key);
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
		swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"uedit32.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"%s/%d/1", pszFilename, lineNumber );
	}
};

std::unique_ptr<Editor> DiscoverUltraEdit()
{
	return std::make_unique<UltraEdit>();
}


