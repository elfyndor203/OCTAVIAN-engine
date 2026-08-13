#include "directories/directories.h"

#include "OCT_Core_eng.h"
#include <direct.h>

void OCT_platform_setCWD(const char* path) {
    if (_chdir(path) != 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "CWD set failed\n");
    }

    printf("Changed CWD to %s\n", path);
}