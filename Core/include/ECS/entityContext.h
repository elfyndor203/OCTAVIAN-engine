#pragma once
#include "layout/types.h"

OCT_handle OCT_entityContext_open(OCT_handle* contextOut);
void OCT_entityContext_dumpEntityPool(OCT_handle contextHandle);