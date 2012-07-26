#include "stdafx.h"
#include "Editor.h"
#include "ZeeException.h"

CHAR szNewFileCookie[] = "New file created by Zee";

bool Editor::CreateFileIfNeeded(LPCWSTR szFullPathName)
{
	if( _waccess(szFullPathName, 0/*exist*/ ) )
	{
		wprintf(L"File \"%s\" does not exist. Create? [Y/N]", szFullPathName);
		CHAR ch = _getch();
		if( ch=='y' || ch=='Y')
		{
			// Create file
			FILE *fileHandle;
			if( _wfopen_s(&fileHandle,szFullPathName,L"wt") != 0 )
			{
				ZeeException exception(L"a", L"Cannot open file \"%s\"", szFullPathName );
				throw exception;
			}

			if (fwrite(szNewFileCookie, 1, sizeof(szNewFileCookie)-1, fileHandle) != sizeof(szNewFileCookie)-1) // -1 to not write out trailing 0
			{
				fclose( fileHandle );
				ZeeException exception(L"a", L"Cannot write into file \"%s\"", szFullPathName );
				throw exception;
			}

			fclose( fileHandle );
		}
		else
		{
			// User did not want to create a file
			return false;
		}
	}
	return true;
}

extern bool g_ForceSetup;

void Editor::OpenFile(LPCWSTR fileName, int lineNumber )
{
	if( g_ForceSetup )
	{
		// Do nothing, se we're in setup mode. Ugly.
		return;
	}

    WCHAR szFullPathName[MAX_PATH];
    if( wcslen(fileName) > 1 && (fileName[1] == L':' || fileName[0] == L'\\') )
    {
        // This is a full path
        wcscpy_s(szFullPathName, arraylen(szFullPathName), fileName);
    }
    else
    {
        WCHAR szCurrentDir[MAX_PATH];
        GetCurrentDirectory(arraylen(szCurrentDir),szCurrentDir);
        _snwprintf_s(szFullPathName, arraylen(szFullPathName), arraylen(szFullPathName), L"%s\\%s", szCurrentDir, fileName);
    }

	if( !CreateFileIfNeeded(szFullPathName) )
	{
		return;
	}

	WCHAR szExe[MAX_PATH];
	this->GetExecutable(szExe, arraylen(szExe));

	WCHAR szFileNameWithLineNumber[MAX_PATH];
	this->FormatParameters(szFileNameWithLineNumber, arraylen(szExe), szFullPathName, lineNumber);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	WCHAR szFullCommandLine[MAX_PATH*2];
	swprintf_s(szFullCommandLine, arraylen(szFullCommandLine), L"\"%s\" %s", szExe, szFileNameWithLineNumber );

	// Start the child process.
	if( !CreateProcess( NULL,
		szFullCommandLine,	// Command line
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
		ZeeException exception(L"a", L"Cannot start program '%s'", szExe );
		throw exception;
	}
}
