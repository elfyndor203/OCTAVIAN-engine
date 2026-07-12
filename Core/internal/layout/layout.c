#include "layout/types.h"

OCT_handle OCT_HANDLE_NULL = {OCT_ID_NULL, OCT_ID_NULL, OCT_ID_NULL};

bool OCT_handle_isNULL(OCT_handle handle) {
    if (handle.objectID == OCT_ID_NULL || handle.containerID == OCT_ID_NULL) {
        return true;
    }
    else {
        return false;
    }
}

bool OCT_handle_isEqual(OCT_handle handle1, OCT_handle handle2) {
    if (handle1.objectID == handle2.objectID
        && handle1.containerID == handle2.containerID) {
        return true;
    }
    else {
        return false;
    }
}