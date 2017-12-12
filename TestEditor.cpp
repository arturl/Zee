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

std::unique_ptr<Editor> DiscoverTestEditor()
{
	return std::make_unique<TestEditor>();
}

