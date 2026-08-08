#include "layout/types.h"

bool OCT_local_isNULL(OCT_local handle) {
    if (handle.objectID == OCT_ID_NULL || OCT_global_isNULL(handle.contextHandle)) {
        return true;
    }
    return false;
}

bool OCT_global_isNULL(OCT_global handle) {
    if (handle.objectID == OCT_ID_NULL || handle.systemID == OCT_ID_NULL) {
        return true;
    }
    return false;
}

bool OCT_handle_isEqual(OCT_local handle1, OCT_local handle2) {
    if (handle1.objectID == handle2.objectID
        && handle1.containerID == handle2.containerID) {
        return true;
    }
    else {
        return false;
    }
}