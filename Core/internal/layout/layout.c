
#include <stdio.h>
#include <inttypes.h>

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

bool OCT_local_isEqual(OCT_local handle1, OCT_local handle2) {
    if (handle1.objectID == handle2.objectID
        && handle1.containerID == handle2.containerID
        && OCT_global_isEqual(handle1.contextHandle, handle2.contextHandle)) {
        return true;
    }
    else {
        return false;
    }
}

bool OCT_global_isEqual(OCT_global handle1, OCT_global handle2) {
    if (handle1.objectID == handle2.objectID
        && handle1.containerID == handle2.containerID
        && handle1.systemID == handle2.systemID) {
        return true;
    }
    else {
        return false;
    }
}

void OCT_global_print(OCT_global handle, int indent) {
    printf("%*c", indent, ' ');
    printf("SystemID: %"PRIu64"\n", handle.systemID);
    printf("%*c", indent, ' ');
    printf("Container ID: %"PRIu64"\n", handle.containerID);
    printf("%*c", indent, ' ');
    printf("Object ID: %"PRIu64"\n", handle.objectID);
}

void OCT_local_print(OCT_local handle, int indent) {
    printf("%*c", indent, ' ');
    printf("Context ID: %"PRIu64"\n", handle.contextHandle.objectID);
    printf("%*c", indent, ' ');
    printf("Object ID: %"PRIu64"\n", handle.objectID);
}
