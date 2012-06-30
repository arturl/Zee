#include "stdafx.h"
#include "Editor.h"
#include "EditorBuilder.h"
#include "CommandLineParser.h"
#include "ZeeException.h"

int wmain(int argc, LPWSTR argv[])
{
	try
	{
		CommandLineParser commandLineParser;

		EditorBuilder editorBuilder;
		Editor& editor = editorBuilder.GetDefaultEditor();

		editor.OpenFile(commandLineParser.GetFilename(), commandLineParser.GetLineNumber());

	}
	catch(ZeeException& exception)
	{
		exception.PrintExplanation();
		return 1;
	}

	return 0;
}

