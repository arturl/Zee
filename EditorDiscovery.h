#pragma once

typedef Editor *(__cdecl *PFNEditorFactory)(void);

#define DAT(X,Y) Editor* Y();
#include "Editors.dat"
#undef DAT

