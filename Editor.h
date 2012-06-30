#pragma once

class Editor
{
protected:
	virtual void GetExecutable(LPWSTR pszExe, int exeSize) = 0;
	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber) = 0;
public:
	virtual bool IsInstalled() = 0;
	virtual void OpenFile(LPCWSTR fileName, int lineNumber );
	static bool CreateFileIfNeeded(LPCWSTR fileName);
};
