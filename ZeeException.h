class ZeeException
{
	WCHAR explanation_[256];
public:

	ZeeException(LPWSTR szCaption, LPWSTR szFormat,...)
	{
		WCHAR szBuffer[1024];
		va_list marker;
		va_start( marker, szFormat );

		int n = _vsnwprintf_s ( szBuffer, arraylen(szBuffer), arraylen(szBuffer), (LPWSTR) szFormat, marker );
		if( n < 0 ) {
			szBuffer[ arraylen(szBuffer)-4 ] = L'\0';
			wcscat_s(szBuffer, arraylen(szBuffer), L"...");
		}
		wcscpy_s(explanation_,arraylen(explanation_),szBuffer);
	}

	ZeeException(LPWSTR explanation)
	{
		wcscpy_s(explanation_,arraylen(explanation_),explanation);
	}
	void PrintExplanation()
	{
		wprintf(L"%s", explanation_);
	}

    void ShowExplanation()
	{
		MessageBoxW(NULL, explanation_, L"Zee", MB_ICONHAND);
	}
};
