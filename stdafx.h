// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define arraylen(a) ((sizeof(a))/(sizeof(a[0])))

#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <windows.h>
#include <crtdbg.h>
#include <memory>
#include <algorithm>

// TODO: reference additional headers your program requires here
