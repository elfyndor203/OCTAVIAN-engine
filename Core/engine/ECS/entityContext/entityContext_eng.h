#pragma once
#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

eOCT_pool* eOCT_getComponentPool(OCT_ID contextID, eOCT_componentDescription component);
eOCT_pool* eOCT_getFieldSourcePool(OCT_ID contextID, eOCT_fieldRequest field);