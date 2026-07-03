#pragma once

static iOCT_errorLevels iOCT_errorLevelList[] = {
#define X(code, type, message) type,
    iOCT_ERROR_LIST
#undef X
};

static char* iOCT_errorMessageList[] = {
#define X(code, type, message) message,
    iOCT_ERROR_LIST
#undef X
};


