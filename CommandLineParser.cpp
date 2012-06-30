#include "stdafx.h"
#include "CommandLineParser.h"
#include "ZeeException.h"

enum ParsingPhase
{
	eStart,
	eModuleName,
	eDelimeter1,
	eFilename,
	eDelimeter2,
	eLinenumber
};

static const LPWSTR g_szSyntaxError = L"Syntax:\nz.exe filename [linenumber]";
static const LPWSTR g_szParsingError = L"Error parsing command line";
static const LPWSTR g_szShowUsage = 
L"Syntax:\n"
L" z.exe filename [linenumber]\n"
L"Other possible uses:\n"
L" z.exe filename(linenumber)\n"
L" z.exe filename:linenumber\n"
L" z.exe /setup\n";

void PrintAllSupportedEditors();

void ShowUsage()
{
	wprintf(
L"Zee Editor Launcher\n"
L"Supports the following editors:\n"
	);
	PrintAllSupportedEditors();
}

extern bool g_ForceSetup;

CommandLineParser::CommandLineParser()
{
	LPCWSTR pszCommandLine = GetCommandLineW();
	size_t commandLineLen = wcslen(pszCommandLine);

	LPWSTR pszFileName = &fileName_[0];
	memset(fileName_,0,sizeof(fileName_));

	WCHAR szLinenumber[MAX_PATH];
	LPWSTR pszLinenumber = &szLinenumber[0];
	memset(szLinenumber,0,sizeof(szLinenumber));

	ParsingPhase phase = eStart;

	bool bInString = false;
	bool bDone = false;
	bool bError = false;
	while(!bDone)
	{
		WCHAR ch = *pszCommandLine++;
		switch(ch)
		{
		case L'"':
			switch(phase)
			{
			case eStart:
				phase = eModuleName;
				bInString = true;
				break;
			case eModuleName:
				// fall through
			case eFilename:
				bInString = !bInString;
				break;
			case eDelimeter1:
				bInString = !bInString;
				phase = eFilename;
				break;
			case eDelimeter2:
				// skip it
				break;
			case eLinenumber:
				bDone = true;
				break;
			}
			break;
		case ' ':
		case ':':
		case '(':
			switch(phase)
			{
			case eStart:
				if( ch == L' ' )
				{
					// just skip it
				}
				else
				{
					bError = bDone = true;
				}
				break;
			case eModuleName:
				if( ch == L' ' )
				{
					phase = eDelimeter1;
				}
				break;
			case eFilename:
				if( bInString )
				{
					*pszFileName++ = ch;
				}
				else
				{
					// This could be drive
					if( ch == ':' && pszFileName - fileName_ == 1 && *pszCommandLine == L'\\' )
					{
						*pszFileName++ = ch;
					}
					else
					{
						phase = eDelimeter2;
					}
				}
				break;
			case eDelimeter1:
			case eDelimeter2:
				// skip it
				break;
			case eLinenumber:
				bDone = true;
				break;
			}
			break;
		case L'0':
		case L'1':
		case L'2':
		case L'3':
		case L'4':
		case L'5':
		case L'6':
		case L'7':
		case L'8':
		case L'9':
			switch(phase)
			{
			case eStart:
				phase = eModuleName;
				*pszFileName++ = ch;
				break;
			case eModuleName:
				// skip it
				break;
			case eDelimeter1:
				phase = eFilename;
				// fall through
			case eFilename:
				*pszFileName++ = ch;
				break;
			case eDelimeter2:
				phase = eLinenumber;
				// fall through
			case eLinenumber:
				*pszLinenumber++ = ch;
				break;
			}
			break;
		case L'\\':
			if( *pszCommandLine == L'"' )
			{
				ch = L'"';
				pszCommandLine++; // skip it over
			}
			// fall through
		default:
			switch(phase)
			{
			case eStart:
				phase = eModuleName;
				break;
			case eModuleName:
				// skip it
				break;
			case eDelimeter1:
				phase = eFilename;
				// fall through
			case eFilename:
				*pszFileName++ = ch;
				break;
			case eDelimeter2:
				bError = bDone = true;
				break;
			case eLinenumber:
				bDone = true;
				break;
			}
			break;
		case '\0':
			bDone = true;
			break;
		}
	}

	lineNumber_ = _wtoi(szLinenumber);

	//wprintf(L"filename: '%s'\n",fileName_);
	//wprintf(L"line number: %d\n",lineNumber_);

	if(wcslen(fileName_)==0)
	{
		ZeeException exception(g_szSyntaxError);
		throw exception;
	}

	if(wcscmp(fileName_,L"-?")==0 || wcscmp(fileName_,L"/?")==0)
	{
		ShowUsage();
		ZeeException exception(g_szShowUsage);
		throw exception;
	}

	if(wcscmp(fileName_,L"-setup")==0 || wcscmp(fileName_,L"/setup")==0)
	{
		g_ForceSetup = true;
	}

	if( bError )
	{
		ZeeException exception(g_szParsingError);
		throw exception;
	}
};

LPCWSTR CommandLineParser::GetFilename()
{
	return this->fileName_;
}

int CommandLineParser::GetLineNumber()
{
	return this->lineNumber_;
}
