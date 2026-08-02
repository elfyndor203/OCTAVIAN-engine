#pragma once
#include "types_eng.h"

#include <stdbool.h>
#include <stdio.h>

enum iOCT_errorLevels{
    iOCT_FATAL,
    iOCT_ERROR,
    iOCT_WARNING,
    iOCT_NOTE
};

#define iOCT_ERROR_LIST \
X(OCT_EXIT_INIT_FAILED,					iOCT_FATAL,		"Init function failed") \
X(OCT_EXIT_FAILED_TO_ALLOCATE,			iOCT_FATAL,		"Memory allocation failed") \
X(OCT_EXIT_FAILED_TO_OPEN_FILE,			iOCT_FATAL,		"Failed to open file") \
X(OCT_EXIT_REFERENCE_DOES_NOT_EXIST,	iOCT_FATAL,		"Reference does not exist") \
X(OCT_EXIT_STALE_HANDLE,				iOCT_FATAL,		"Handle is no longer valid") \
X(OCT_EXIT_STALE_REFERENCE,             iOCT_FATAL,		"Reference is no longer valid") \
X(OCT_EXIT_NULL_HANDLE,                 iOCT_FATAL,		"Null handle") \
X(OCT_EXIT_NOT_YET_IMPLEMENTED,			iOCT_FATAL,		"Feature is not yet implemented") \
X(OCT_EXIT_ACCESS_DENIED,				iOCT_FATAL,		"Access denied") \
X(OCT_EXIT_OUT_OF_BOUNDS, 				iOCT_FATAL,		"Out of bounds") \
X(OCT_EXIT_SOURCE_MISMATCH,             iOCT_FATAL,		"Source mismatch") \
X(OCT_EXIT_REGISTRATION_FAILED,         iOCT_FATAL,		"Registration failed") \
\
X(OCT_ERR_DIVIDE_BY_ZERO,				iOCT_ERROR,		"Division by zero") \
X(OCT_ERR_CREATION_FAILED,              iOCT_ERROR,       "Intended creation failed")\
X(OCT_ERR_NOT_PROVIDED,				    iOCT_ERROR,		"Not provided") \
\
X(OCT_WARNING_UNINITIALIZED,			iOCT_WARNING,	"Accessing uninitialized object") \
\
X(OCT_NOTE_POOL_NOT_EMPTY,              iOCT_NOTE,      "Pool is not empty")

void iOCT_logError(OCT_ERROR_CODES errorCode);  // for macro access
bool iOCT_handleError(OCT_ERROR_CODES errorCode); // for macro access

#define OCT_ERROR_LOG(errorCode, reason) do {\
    iOCT_logError(errorCode); \
    fprintf(stderr, "Source: \n  File: %s\n  Line: %d\n  Function: %s\n  Message: %s\n", __FILE__, __LINE__, __func__, reason); \
    iOCT_handleError(errorCode); \
} while (0)

enum OCT_ERROR_CODES {
#define X(code, type, message) code,	// defines the macro once
    iOCT_ERROR_LIST						// expand
#undef X								// undef for next enum
};

