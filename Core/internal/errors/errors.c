#include "errors/errors_eng.h"
#include "errors_int.h"
#include "errors/types_eng.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void iOCT_logError(OCT_ERROR_CODES errorCode) {
	iOCT_errorLevels errorLevel = iOCT_errorLevelList[errorCode];
	const char* message = iOCT_errorMessageList[errorCode];

	printf("\n!!!\n");
	if (errorLevel == iOCT_FATAL) {
		printf("FATAL Error %d: ", errorCode);
	}
	else if (errorLevel == iOCT_ERROR) {
		printf("ERROR %d: ", errorCode);
	}
	else if (errorLevel == iOCT_WARNING) {
		printf("WARNING %d: ", errorCode);
	}
	else {
		printf("NOTE %d: ", errorCode);
	}
	printf("%s\n", message);
}

bool iOCT_handleError(OCT_ERROR_CODES errorCode) {
	iOCT_errorLevels errorLevel = iOCT_errorLevelList[errorCode];
	if (errorLevel == iOCT_FATAL) {
		exit(errorCode);
	}

	if (errorLevel == iOCT_ERROR) {
		return true;
	}

	return false;
}