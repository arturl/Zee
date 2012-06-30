#include "stdafx.h"
#include "Editor.h"

class Notepad : public Editor
{
	virtual bool IsInstalled()
	{
		return true;
	}

	virtual void GetExecutable(LPWSTR pszExe, int exeSize)
	{
		WCHAR szWindir[MAX_PATH];
		GetEnvironmentVariableW( L"WINDIR", szWindir, arraylen(szWindir));
		swprintf_s(pszExe, exeSize, L"%s\\%s", szWindir, L"notepad.exe" );
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
		swprintf_s(pszParams, paramSize, L"\"%s\"", pszFilename );
	}
};

Editor* DiscoverNotepad()
{
	return new Notepad();
}

