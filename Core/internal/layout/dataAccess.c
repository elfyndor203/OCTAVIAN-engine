#include "OCT_Core_eng.h"

#include "ECS/entityContext_int.h"

eOCT_pool* eOCT_field_getSourcePool(OCT_handle contextHandle, eOCT_fieldTicket fieldTicket) {
    if (fieldTicket.global && fieldTicket.globalPool) {
        return fieldTicket.globalPool;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_COMPONENT) {
        iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
        eOCT_pool* componentPool = &((eOCT_pool*)context->componentPools.array)[fieldTicket.providerTypeIndex];
        return componentPool;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_DATAPOOL) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Data pools are being updated\n");
        return NULL;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_EVENT) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
        return NULL;
    }

    if (fieldTicket.providerType == eOCT_DATAPATTERN_SINGLE) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context singles are not yet implemented\n");
        return NULL;
    }

    return NULL;
}

void* eOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex) {
    void* fieldLoc = eOCT_pool_access(&sourcePool, entryIndex, fieldDetails.offsetFromStruct);
    return fieldLoc;
}

eOCT_pool* eOCT_component_getPool(OCT_handle contextHandle, eOCT_componentKey componentKey) {
    iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

    eOCT_pool* sourcePool = &((eOCT_pool*)context->componentPools.array)[componentKey.componentTypeIndex];
    return sourcePool;
}

eOCT_pool* eOCT_event_getPool(OCT_handle contextHandle, eOCT_eventKey eventKey, eOCT_pool** callbackPoolOut) {
    eOCT_pool* sourcePool;
    eOCT_pool* callbackPool;
    if (eventKey.global && eventKey.globalEventPool) {
        sourcePool = eventKey.globalEventPool;
        callbackPool = eventKey.globalCallbackPool;
    }
    else {
        iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
        sourcePool = &((eOCT_pool*)context->eventManager.eventPools.array)[eventKey.eventTypeIndex];
        callbackPool = &((eOCT_pool*)context->eventManager.callbackPools.array)[eventKey.eventTypeIndex];
    }

    if (callbackPoolOut) {
        *callbackPoolOut = callbackPool;
    }
    return sourcePool;
}