#include "errorHandling_internal.h"

bool logError(errorCodes errorCode) {
	if (errorCode <= FATAL_THRESHOLD) {
		printf("FATAL Error: %d\n", errorCode);
		if (errorCode <= ERROR_THRESHOLD) {
			exit(errorCode);
		}
		return true;
	}

	else if (errorCode <= WARNING_THRESHOLD) {
		printf("Error: %d\n", errorCode);
		return true;
	}

	else {
		printf("Warning: %d\n", errorCode);
		return false;
	}
}

/// API

bool OCT_logError(errorCodes errorCode) {
	return logError(errorCode);
}