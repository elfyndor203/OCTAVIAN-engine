#include "types_int.h"
#include "registry_int.h"
#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "ECS/entityContext_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"
#include "scheduler/scheduler_int.h"
#include "globals/globals_int.h"

static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut);
static void iOCT_registry_registerComponent(eOCT_componentDescription* componentDesc);
static void iOCT_registry_registerEvent(eOCT_eventDescription* eventDesc);
static void iOCT_registry_registerDataPool(eOCT_dataPoolDescription* dataPoolDesc);
static void iOCT_registry_registerSingle(eOCT_singleDescription* singleDesc);
static void iOCT_registry_registerField(eOCT_fieldDescription* field, OCT_index fieldNum, OCT_ID systemID, OCT_index providerIndex, bool global);
static OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, OCT_index providerIndex, bool global);
static eOCT_pool* iOCT_registry_findGlobalPool(eOCT_fieldDescription field);
iOCT_registry iOCT_registry_inst = { 0 }; 

#pragma region internal
void init_OCT_registry_init() {
	eOCT_pool systems = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_systemDescription*)); // store system pointers
	eOCT_pool components = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_componentDescription));
	eOCT_pool events = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_eventDescription));
	eOCT_pool dataPools = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataPoolDescription));
	eOCT_pool globalSingles = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_singleDescription*));
	eOCT_pool fields = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_fieldDescription));
	iOCT_registry_inst.systems = systems;
	iOCT_registry_inst.components = components;
	iOCT_registry_inst.dataPools = dataPools;
	iOCT_registry_inst.events = events;
	iOCT_registry_inst.globalSingles = globalSingles;
	iOCT_registry_inst.fields = fields;
	iOCT_registry_inst.success = true;

	printf("| Registry initialized\n");
}

void init_OCT_registry_distributeFields() {
	eOCT_pool systemPool = iOCT_registry_inst.systems;
	eOCT_systemDescription** systemArray = (eOCT_systemDescription**)systemPool.array;

	for (int systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		eOCT_systemDescription* system = systemArray[systemCtr];
		eOCT_pool requestPool = system->requestedFields;

		if (eOCT_pool_isEmpty(requestPool)) {	// ensure pool has contents
			continue;
		}
		eOCT_fieldRequest* requestArray = (eOCT_fieldRequest*)requestPool.array;

		// for each request
		for (int requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			eOCT_fieldRequest* request = &requestArray[requestCtr];
			eOCT_fieldTicket* ticket = request->ticketCacheLocation;
			eOCT_fieldDescription match;
			if (iOCT_registry_findField(request->name, &match)) {	// if there is a match
				ticket->name = match.name;
				ticket->type = match.type;
				ticket->global = match.global_reg;
				ticket->providerType = match.providerType;
				ticket->offsetFromStruct = match.offset;
				ticket->providerIndex = match.providerIndex_reg;
				ticket->globalPool = iOCT_registry_findGlobalPool(match);

				request->fulfilled_reg = true;
				request->providerType_reg = match.providerType;
				request->global_reg = match.global_reg;
				request->fieldOffset_reg = match.offset;
				request->providerIndex_reg = match.providerIndex_reg;
				//printf("Fulfilled field '%s' for system '%s'\n", request->name, system->name);
			}
			else {
				//printf("Failed to find existing field '%s'\n", request->name);
				request->fulfilled_reg = false;
				if (!request->optional) {
					iOCT_registry_inst.success = false;
				}
			}
		}
	}
}

void init_OCT_registry_initAllSystems() {
	printf("INIT ALL SYSTEMS\n");
	eOCT_systemDescription system;
	eOCT_systemInitFx initFx;
	eOCT_systemUpdateFx updateFx;
	for (OCT_index systemCtr = 0; systemCtr < iOCT_registry_inst.systems.count; systemCtr++) {
		system = **(eOCT_systemDescription**)eOCT_pool_access(&iOCT_registry_inst.systems, systemCtr, 0);
		initFx = system.initFx;
		if (initFx) {
			printf("Init system %s with INIT fx %p\n", system.name, initFx);
			initFx();
		}
		else {
			printf("System %s has no INIT fx\n", system.name);
		}
	}
}

void init_OCT_registry_check() {
	eOCT_pool systemPool = iOCT_registry_inst.systems;
	eOCT_systemDescription** systemArray = (eOCT_systemDescription**)systemPool.array;
	eOCT_systemDescription system;
	int systemCtr = 0;

	eOCT_pool componentPool;
	eOCT_componentDescription* componentArray;
	eOCT_componentDescription component;
	int componentCtr = 0;

	eOCT_pool requestPool;
	eOCT_fieldRequest* requestArray;
	eOCT_fieldRequest request;
	int requestCtr = 0;

	printf("\n--------SUMMARY--------\n");
	printf("Systems: (%d - %d reserved)\n", 1, OCT_ID_SYSTEM_START - 1);
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = *systemArray[systemCtr];
		printf("%02zu.    | %s\n", system.systemID_reg, system.name);
	}

	printf("\nComponents:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = *systemArray[systemCtr];
		componentPool = system.providedComponents;
		componentArray = (eOCT_componentDescription*)componentPool.array;
		for (componentCtr = 0; componentCtr < componentPool.count; componentCtr++) {
			component = componentArray[componentCtr];
			printf("%02zu.    | %s\n", component.componentTypeIndex_reg, component.name);
		}
	}
	printf("ECS Component Count: %zu\n", iOCT_registry_inst.components.count);
	printf("ECS Entity Size: %zu\n", iOCT_ECS_inst.entitySize);

	printf("\nFields requested:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = *systemArray[systemCtr];
		requestPool = system.requestedFields;
		requestArray = (eOCT_fieldRequest*)requestPool.array;
		for (requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			request = requestArray[requestCtr];
			if (request.fulfilled_reg) {
				printf("%02zu[%02zu]", request.providerIndex_reg, request.fieldOffset_reg);
			}
			else if (request.optional) {
				printf("XX[XX]");
			}
			else {
				printf("%7c| [FAIL]", ' ');
			}
			printf(" | %s\n", request.name);
		}
	}
	printf("\nStatus: ");
	if (iOCT_registry_inst.success) {
		printf("Success\n");
	}
	else {
		printf("Failed\n");
		OCT_ERROR_LOG(OCT_EXIT_REGISTRATION_FAILED, "Check registration log to find failure");
	}
	printf("-----------------------\n\n");

}

#pragma endregion

#pragma region generators
eOCT_pool eOCT_generateFieldDescriptionPool(eOCT_fieldDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, count, sizeof(eOCT_fieldDescription));
	eOCT_fieldDescription* destination;
	for (OCT_index ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}
eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, count, sizeof(eOCT_componentDescription));

	eOCT_componentDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_componentDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}
eOCT_pool eOCT_generateDataPoolDescriptionPool(eOCT_dataPoolDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, count, sizeof(eOCT_dataPoolDescription));

	eOCT_dataPoolDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_dataPoolDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}
eOCT_pool eOCT_generateEventDescriptionPool(eOCT_eventDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, count, sizeof(eOCT_eventDescription));

	eOCT_eventDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_eventDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}

	return pool;
}
eOCT_pool eOCT_generateSingleDescriptionPool(eOCT_singleDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_REGISTRY, count, sizeof(eOCT_singleDescription));

	eOCT_singleDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_singleDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}

	return pool;
}
eOCT_pool eOCT_generateFieldRequestPool(eOCT_fieldRequest* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_NULL, count, sizeof(eOCT_fieldRequest));

	eOCT_fieldRequest* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

#pragma endregion
/*!
 * Registers each system's components, data pools, and events into the registry. For each component, data pool, and event, registers each of its fields. Engine init fails if any duplicate fields are found.
 * @param systemDescription
 */
void eOCT_registry_registerSystem(eOCT_systemDescription* systemDescription) {
	OCT_ID systemID = iOCT_registry_inst.systems.count + OCT_ID_SYSTEM_START;

	eOCT_systemDescription** destination = (eOCT_systemDescription**)eOCT_pool_addEntry(&iOCT_registry_inst.systems, NULL);	// addEntry after so the ID starts at 3 instead of 3 + 1
	*destination = systemDescription;
	systemDescription->systemID_reg = systemID;
	printf("\n--------------------------------\n");
	printf("%02"PRIu64".--.--| System '%s':\n", systemDescription->systemID_reg, systemDescription->name);

	// COMPONENTS
	if (eOCT_pool_isEmpty(systemDescription->providedComponents)) {		// requests handed separately later, so registration ends
		printf("No provided components\n");
	}
	else {
		eOCT_componentDescription* componentArray = (eOCT_componentDescription*)systemDescription->providedComponents.array;	// register all components and all of their fields
		for (OCT_index componentCtr = 0; componentCtr < systemDescription->providedComponents.count; componentCtr++) {
			eOCT_componentDescription* component = &componentArray[componentCtr];
			printf("%02"PRIu64".%02zu.--| %2cComponent %zu: %-15s\n", systemID, component->componentTypeIndex_reg, ' ', component->componentTypeIndex_reg, component->name);

			iOCT_registry_registerComponent(component);
			iOCT_registry_registerFields(component->providedFields, systemID, component->componentTypeIndex_reg, false);
		}
	}

	printf("\n");
	// EVENTS
	if (eOCT_pool_isEmpty(systemDescription->providedEvents)) {
		printf("No provided events\n");
	}
	else {
		eOCT_eventDescription* eventArray = (eOCT_eventDescription*)systemDescription->providedEvents.array;
		for (OCT_index eventCtr = 0; eventCtr < systemDescription->providedEvents.count; eventCtr++) {
			eOCT_eventDescription* event = &eventArray[eventCtr];
			printf("%02"PRIu64".%02zu.--| %2cEvent %zu: %-15s\n", systemID, event->eventTypeIndex_reg, ' ', event->eventTypeIndex_reg, event->name);

			iOCT_registry_registerEvent(event);
			iOCT_registry_registerFields(event->providedFields, systemID, event->eventTypeIndex_reg, event->global);
		}
	}

	printf("\n");
	// DATA POOLS
	if (eOCT_pool_isEmpty(systemDescription->providedDataPools)) {
		printf("No additional provided data\n");
	}
	else {
		eOCT_dataPoolDescription* dataPoolArray = (eOCT_dataPoolDescription*)systemDescription->providedDataPools.array;
		for (OCT_index dataPoolCtr = 0; dataPoolCtr < systemDescription->providedDataPools.count; dataPoolCtr++) {
			eOCT_dataPoolDescription* dataPool = &dataPoolArray[dataPoolCtr];
			printf("%02"PRIu64".%02zu.--| %2cData Pool %zu: %-15s\n", systemID, dataPool->dataPoolTypeIndex_reg, ' ', dataPool->dataPoolTypeIndex_reg, dataPool->name);

			iOCT_registry_registerDataPool(dataPool);
			iOCT_registry_registerFields(dataPool->providedFields, systemID, dataPool->dataPoolTypeIndex_reg, dataPool->global);
		}
	}

	printf("\n");
	// SINGLES'
	if (eOCT_pool_isEmpty(systemDescription->providedSingles)) {
		printf("No provided singles\n");
	}
	else {
		eOCT_singleDescription* singlesArray = (eOCT_singleDescription*)systemDescription->providedSingles.array;
		for (OCT_index singleCtr = 0; singleCtr < systemDescription->providedSingles.count; singleCtr++) {
			eOCT_singleDescription* single = &singlesArray[singleCtr];
			printf("%02"PRIu64".%02zu.--| %2cGlobal %zu: %-15s\n", systemID, single->singleTypeIndex_reg, ' ', single->singleTypeIndex_reg, single->name);
			if (single->providedField.offset != 0) {
				OCT_ERROR_LOG(OCT_EXIT_REGISTRATION_FAILED, "Singles must have offset 0");
				return;
			}

			iOCT_registry_registerSingle(single);
			iOCT_registry_registerField(&single->providedField, 0, systemID, single->singleTypeIndex_reg, single->global);
		}
	}
	printf("--------------------------------\n\n");
}

#pragma region navigation
eOCT_eventDescription iOCT_registry_findSourceEventDescription(eOCT_fieldRequest fieldRequest) {
	eOCT_eventDescription* array = (eOCT_eventDescription*)iOCT_registry_inst.events.array;
	return array[fieldRequest.providerIndex_reg];
}
#pragma endregion

#pragma region static
static void iOCT_registry_registerComponent(eOCT_componentDescription* componentDesc) {
	OCT_index componentIndex;
	eOCT_componentDescription* registryEntry = (eOCT_componentDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.components, &componentIndex);	// store a stable copy in the registry
	componentDesc->componentTypeIndex_reg = componentIndex;	// inform the system of its component's index
	*registryEntry = *componentDesc;						// THEN copy

	if (componentDesc->cacheLocation) {
		*componentDesc->cacheLocation = *componentDesc;
	}
}
static void iOCT_registry_registerEvent(eOCT_eventDescription* eventDesc) {
	OCT_index eventIndex;
	eOCT_eventDescription* registryEntry = (eOCT_eventDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.events, &eventIndex);
	eventDesc->eventTypeIndex_reg = eventIndex;
	*registryEntry = *eventDesc;

	if (eventDesc->cacheLocation) {
		*eventDesc->cacheLocation = *eventDesc;
	}
}
static void iOCT_registry_registerDataPool(eOCT_dataPoolDescription* dataPoolDesc) {
	dataPoolDesc->dataPoolTypeIndex_reg = iOCT_ECS_addDataPool(*dataPoolDesc, dataPoolDesc->global);

	eOCT_dataPoolDescription* registryEntry = (eOCT_dataPoolDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.dataPools, NULL);
	*registryEntry = *dataPoolDesc;

	if (dataPoolDesc->cacheLocation) {
		*dataPoolDesc->cacheLocation = *dataPoolDesc;
	}
}
static void iOCT_registry_registerSingle(eOCT_singleDescription* singleDesc) {
	assert(singleDesc->global && "Context local singles not yet implemented");
	OCT_index singleIndex;
	eOCT_singleDescription* registryEntry = (eOCT_singleDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.globalSingles, &singleIndex);
	singleDesc->singleTypeIndex_reg = singleIndex;
	*registryEntry = *singleDesc;

	if (singleDesc->cacheLocation) {
		*singleDesc->cacheLocation = *singleDesc;
	}
}
static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut) {
	eOCT_pool* fields = &iOCT_registry_inst.fields;
	//printf("Number of fields in registry: %d\n", fields->count);
	eOCT_fieldDescription* fieldArray = (eOCT_fieldDescription*)fields->array;
	eOCT_fieldDescription targetField;
	int fieldCtr = 0;

	for (fieldCtr = 0; fieldCtr < fields->count; fieldCtr++) {		// check every field in the registry
		targetField = fieldArray[fieldCtr];
		if (strcmp(targetField.name, fieldName) == 0) {
			if (fieldOut) {
				*fieldOut = targetField;
			}
			return true;
		}
	}
	return false;
}
static OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, OCT_index providerIndex, bool global) {
	if (eOCT_pool_isEmpty(providedFields)) {
		printf("%13c No public fields\n", ' ');
		return 0;
	}

	eOCT_fieldDescription* fieldArray = (eOCT_fieldDescription*)providedFields.array;
	eOCT_fieldDescription* field;
	eOCT_fieldDescription* fieldDestination;
	OCT_index fieldCtr = 0;
	for (fieldCtr = 0; fieldCtr < providedFields.count; fieldCtr++) {
		field = &fieldArray[fieldCtr];
		iOCT_registry_registerField(field, fieldCtr, systemID, providerIndex, global);
	}

	printf("%13c Fields: %zu\n", ' ', providedFields.count);
	return fieldCtr;
}

static void iOCT_registry_registerField(eOCT_fieldDescription* field, OCT_index fieldNum, OCT_ID systemID, OCT_index providerIndex, bool global) {
	printf("%02"PRIu64".%02zu.%02zu| %4cField: %-15s | ", systemID, providerIndex, fieldNum, ' ', field->name);

	if (iOCT_registry_findField(field->name, NULL)) {	// check for duplicates
		printf("Failed: Field already exists\n");
		iOCT_registry_inst.success = false;
	}

	else {
		field->providerIndex_reg = providerIndex;
		field->global_reg = global;

		eOCT_fieldDescription* fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.fields, NULL);	// add field to the registry
		*fieldDestination = *field;
		printf("Success\n");
	}
}
static eOCT_pool* iOCT_registry_findGlobalPool(eOCT_fieldDescription field) {
	if (!field.global_reg) {
		return NULL;
	}

	eOCT_pool* pool;
	switch (field.providerType) {
	case eOCT_FIELDPROVIDER_COMPONENT:
		return NULL;
	case eOCT_FIELDPROVIDER_DATAPOOL:
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Data pools are being updated\n");
		return NULL;
	case eOCT_FIELDPROVIDER_EVENT:
		pool = &((eOCT_pool*)iOCT_globals_inst.globalEvents.eventPools.array)[field.providerIndex_reg];
		break;
	case eOCT_FIELDPROVIDER_SINGLE:
		pool = &iOCT_globals_inst.globalSingles;
		break;
	default:
		return NULL;
	}

	return pool;
}
#pragma endregion

eOCT_pool* eOCT_field_getSourcePool(OCT_handle contextHandle, eOCT_fieldTicket fieldDetails) {
	if (fieldDetails.global && fieldDetails.globalPool) {
		return fieldDetails.globalPool;
	}

	if (fieldDetails.providerType == eOCT_FIELDPROVIDER_COMPONENT) {
		iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
		eOCT_pool* componentPool = &((eOCT_pool*)context->componentPools.array)[fieldDetails.providerIndex];
		return componentPool;
	}

	if (fieldDetails.providerType == eOCT_FIELDPROVIDER_DATAPOOL) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Data pools are being updated\n");
		return NULL;
	}

	if (fieldDetails.providerType == eOCT_FIELDPROVIDER_EVENT) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context events not yet implemented");
		return NULL;
	}

	if (fieldDetails.providerType == eOCT_FIELDPROVIDER_SINGLE) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Context singles are not yet implemented\n");
		return NULL;
	}

	return NULL;
}

void* eOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex) {
	void* fieldLoc = eOCT_pool_access(&sourcePool, entryIndex, fieldDetails.offsetFromStruct);
	return fieldLoc;
}