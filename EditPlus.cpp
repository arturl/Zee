#include "stdafx.h"
#include "Editor.h"

class EditPlus : public Editor
{
	WCHAR szPath_[_MAX_PATH];
	bool installed_;

public:
	EditPlus()
	{
		installed_ = false;
		HKEY key;
		LPCWSTR szEditPlusHive = L"Software\\ES-Computing\\EditPlus 3\\Install";
		LONG lRet = RegOpenKeyW(HKEY_CURRENT_USER, szEditPlusHive, &key);
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
		swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"editplus.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"-e \"%s\" -cursor %d:1", pszFilename, lineNumber );
	}
};

std::unique_ptr<Editor> DiscoverEditPlus()
{
	return std::make_unique<EditPlus>();
}


