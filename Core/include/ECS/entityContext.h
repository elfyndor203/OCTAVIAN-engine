#pragma once
#include "layout/types.h"

OCT_global OCT_entityContext_open(OCT_local* rootOut);
OCT_local OCT_entityContext_getRoot(OCT_global context);
void OCT_entityContext_dumpEntityPool(OCT_global contextHandle);