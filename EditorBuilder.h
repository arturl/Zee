#pragma once
#include "Editor.h"
#include "EditorDiscovery.h"

class EditorBuilder
{
	PFNEditorFactory* methods_;
	int numMethods_;
	Editor* defaultEditor_;

	void GetFactoryMethods();

public:
	EditorBuilder();
	~EditorBuilder();
	Editor& GetDefaultEditor();
};
