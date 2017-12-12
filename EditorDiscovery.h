#pragma once

typedef std::unique_ptr<Editor> (__cdecl *PFNEditorFactory)(void);

#define DAT(X,Y) std::unique_ptr<Editor> Y();
#include "Editors.dat"
#undef DAT

