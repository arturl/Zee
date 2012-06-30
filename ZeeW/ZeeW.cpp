// ZeeW.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ZeeW.h"
#include "..\Editor.h"
#include "..\EditorBuilder.h"
#include "..\CommandLineParser.h"
#include "..\ZeeException.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
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
        exception.ShowExplanation();
        return 1;
    }
    return 0;
}
