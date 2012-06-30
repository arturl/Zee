#include "stdafx.h"
#include "Editor.h"

class TestEditor : public Editor
{
	virtual bool IsInstalled()
	{
		return false;
	}

	virtual void GetExecutable(LPWSTR pszExe, int exeSize)
	{
	}

	virtual void FormatParameters(LPWSTR pszParams, int paramSize, LPCWSTR pszFilename, int lineNumber)
	{
	}
};

Editor* DiscoverTestEditor()
{
	return new TestEditor();
}

