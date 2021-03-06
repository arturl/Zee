#pragma once
#include "Editor.h"
#include "EditorDiscovery.h"

class EditorBuilder
{
	PFNEditorFactory* methods_;
	int numMethods_;
	std::unique_ptr<Editor> defaultEditor_;

	void GetFactoryMethods();

public:
	EditorBuilder();
	~EditorBuilder();
	Editor& GetDefaultEditor();
};
