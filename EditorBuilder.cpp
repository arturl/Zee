#include "stdafx.h"
#include "EditorBuilder.h"
#include "ZeeException.h"

EditorBuilder::EditorBuilder()
{
	// Collect all available supportedEditors
	GetFactoryMethods();
	defaultEditor_ = NULL;
}

#define DAT(X,Y) L##X,
static LPCWSTR editorNames[] =
{
#include "Editors.dat"
};
#undef DAT

const int editorCount = arraylen(editorNames);

void PrintAllSupportedEditors()
{
	for( int i=0; i<editorCount; ++i )
	{
		wprintf(L" %s\n",editorNames[i]);
	}
}

#define DAT(X,Y) &Y,

void EditorBuilder::GetFactoryMethods()
{
	static PFNEditorFactory factoryMethods[] =
	{
#include "Editors.dat"
	};

	methods_ = &factoryMethods[0];
	numMethods_ = arraylen(factoryMethods);
}

#undef DAT

bool g_ForceSetup = false;

Editor& EditorBuilder::GetDefaultEditor()
{
	int selectedEditor = -1; // don't have default editor
	HKEY key;
	LPCWSTR szZeeHive = L"Software\\Zee";
	LONG lRet = RegOpenKeyW(HKEY_CURRENT_USER, szZeeHive, &key);
	if( lRet==2 )
	{
		lRet=RegCreateKeyW(HKEY_CURRENT_USER, szZeeHive, &key);
		if( lRet != ERROR_SUCCESS )
		{
			ZeeException exception(L"Cannot access registry");
			throw exception;
		}
	}
	else if( !g_ForceSetup )
	{
		WCHAR data[256];
		DWORD dwType = REG_SZ;
		DWORD dwSize = sizeof(data);
		lRet = RegQueryValueExW(key, L"DefaultEditor", NULL, &dwType, (LPBYTE)data, &dwSize);

		if( lRet == ERROR_SUCCESS )
		{
			// Now look up this editor in the list
			for( int i=0; i<editorCount; ++i )
			{
				if( wcscmp(editorNames[i],data)==0 )
				{
					selectedEditor = i;
					break;
				}
			}
		}
	}

	if( selectedEditor == -1 )
	{
		WCHAR szNumber[8] = L"0";
		if( editorCount==1 )
		{
			// Only one choice, so why ask...
			selectedEditor = 0;
		}
		else
		{
			wprintf(L"Default editor not set, select from the following choices:\n");
			int index=0;
			int indexmap[editorCount] = {-1};
			Editor* supportedEditors[editorCount];

			// List all supported editors and ask for default one
			for( int i=0; i<editorCount; ++i )
			{
				supportedEditors[i] = (methods_[i])();
				if( supportedEditors[i]->IsInstalled() )
				{
					indexmap[index] = i;
					wprintf(L"%d: %s\n", index, editorNames[i]);
					index++;
				}
			}
			wprintf(L">");
			int iSelection = -1;
			int ret = wscanf_s(L"%s", szNumber, arraylen(szNumber));
			if( ret > 0 )
			{
				iSelection = _wtoi(szNumber);
			}
			if( iSelection < 0 || iSelection > index)
			{
				ZeeException exception(L"aa", L"Error: The number must be between 0 and %d", index);
				throw exception;
			}
			selectedEditor = indexmap[iSelection];

			// Delete editors that will not be used
			for( int i=0; i<editorCount; ++i )
			{
				if( i != selectedEditor )
				{
					delete supportedEditors[i];
				}
			}

			defaultEditor_ = supportedEditors[selectedEditor];
			wprintf(L"Setting %s as default editor\n", editorNames[selectedEditor]);
		}

		lRet = RegSetValueExW(key, L"DefaultEditor", 0, REG_SZ, (LPBYTE)editorNames[selectedEditor], (DWORD)wcslen(editorNames[selectedEditor])*2);

		if( lRet != ERROR_SUCCESS )
		{
			ZeeException exception(L"aa", L"Cannot save default editor name in registry");
			throw exception;
		}

		RegCloseKey(key);

	}

	if( defaultEditor_ == NULL )
	{
		defaultEditor_ = (methods_[selectedEditor])();
	}
	return *defaultEditor_;
}

EditorBuilder::~EditorBuilder()
{
	delete defaultEditor_;
}

