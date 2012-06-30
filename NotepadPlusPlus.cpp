#include "stdafx.h"
#include "Editor.h"

class NotepadPlusPlus : public Editor
{
	WCHAR szPath_[_MAX_PATH];
	bool installed_;

public:
	NotepadPlusPlus()
	{
		installed_ = false;
		HKEY key;
		LPCWSTR szNotepadPlusPlusHive = L"SOFTWARE\\Notepad++";
		LONG lRet = RegOpenKeyW(HKEY_LOCAL_MACHINE, szNotepadPlusPlusHive, &key);
		if( lRet == ERROR_SUCCESS ) 
		{
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(szPath_);
			lRet = RegQueryValueExW(key, NULL, NULL, &dwType, (LPBYTE)szPath_, &dwSize);
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
		swprintf_s(pszExe, exeSize, L"%s\\%s", szPath_, L"notepad++.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"\"%s\" -n%d", pszFilename, lineNumber );
	}
};

Editor* DiscoverNotepadPlusPlus()
{
	return new NotepadPlusPlus();
}



