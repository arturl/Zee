class CommandLineParser
{
	WCHAR fileName_[MAX_PATH];
	int   lineNumber_;
public:
	CommandLineParser();
	LPCWSTR GetFilename();
	int     GetLineNumber();
};

