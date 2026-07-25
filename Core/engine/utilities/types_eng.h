#pragma once
#include "layout/types.h"
#include "math/definitions/aliases.h"
typedef struct eOCT_pool eOCT_pool;
typedef struct eOCT_IDMap eOCT_IDMap;

typedef enum eOCT_pool_fillStyles eOCT_pool_fillStyles;
typedef union eOCT_pool_fillValue eOCT_pool_fillValue;
typedef struct eOCT_pool_fillSetting eOCT_pool_fillSetting;
typedef struct eOCT_pool_mapSetting eOCT_pool_mapSetting;
typedef struct eOCT_pool_sortSetting eOCT_pool_sortSetting;

typedef bool (*eOCT_pool_shuffleCallback)(eOCT_IDMap* IDMap, OCT_ID updatedID, OCT_index newIndex);