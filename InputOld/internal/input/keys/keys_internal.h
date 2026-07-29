#pragma once
#include "input/keys/keys.h"

#include "cOCT_EngineStructure.h"

OCT_keyState iOCT_keyState_read(OCT_key key);
void iOCT_keyState_set(OCT_key key, OCT_keyState state);
void iOCT_keyState_convert();