#pragma once

#include "layout/types.h"
#include "registry/registry_eng.h"

typedef struct eOCT_contextToken eOCT_contextToken;
typedef void (*eOCT_rootAttachmentFx)(OCT_handle rootEntity);
typedef void (*eOCT_eventCallbackFx)(OCT_index eventIndex);