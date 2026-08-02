#pragma once

#include "layout/types.h"
#include "registry/registry_eng.h"

/*!
 * A contextToken is needed to access local fields owned by other systems.
 */
typedef struct eOCT_contextToken eOCT_contextToken;
typedef void (*eOCT_rootAttachmentFx)(OCT_handle rootEntity);
typedef void (*eOCT_eventCallbackFx)(OCT_index eventIndex);