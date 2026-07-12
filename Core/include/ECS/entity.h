#pragma once
#include "layout/types.h"

OCT_handle OCT_entity_new(OCT_handle contextHandle);
bool OCT_entity_sameContext(OCT_handle entity1, OCT_handle entity2);
bool OCT_entity_fromContext(OCT_handle entity, OCT_handle context);
