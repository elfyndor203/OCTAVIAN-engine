#include "types_int.h"
#include "registry_int.h"
#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "ECS/ECS_int.h"
#include "utilities/utilities_eng.h"
#include "layout/systems.h"
#include "scheduler/scheduler_int.h"

static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut);
static void iOCT_registry_registerComponent(eOCT_componentDescription* componentDesc);
static void iOCT_registry_registerEvent(eOCT_eventDescription* eventDesc);
static void iOCT_registry_registerDataPool(eOCT_dataPoolDescription* dataPoolDesc);
static OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, eOCT_fieldProvider providerType, OCT_index providerIndex, bool global);
iOCT_registry iOCT_registry_inst = { 0 }; 

#pragma region internal
void init_OCT_registry_init() {
	eOCT_pool systems = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_systemDescription*)); // store system pointers
	eOCT_pool components = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_componentDescription));
	eOCT_pool events = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_eventDescription));
	eOCT_pool dataPools = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataPoolDescription));
	eOCT_pool fields = eOCT_pool_open(OCT_ID_REGISTRY, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_fieldDescription));
	iOCT_registry_inst.systems = systems;
	iOCT_registry_inst.components = components;
	iOCT_registry_inst.dataPools = dataPools;
	iOCT_registry_inst.events = events;
	iOCT_registry_inst.fields = fields;
	iOCT_registry_inst.success = true;

	printf("| Registry initialized\n");
}

void init_OCT_registry_distributeFields() {
	eOCT_pool systemPool = iOCT_registry_inst.systems;
	eOCT_systemDescription** systemArray = (eOCT_systemDescription**)systemPool.array;
	eOCT_systemDescription* system;

	eOCT_pool requestPool;
	eOCT_fieldRequest* requestArray;
	eOCT_fieldRequest* request;

	eOCT_fieldDescription match;
	
	for (int systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = systemArray[systemCtr];
		requestPool = system->requestedFields;

		if (eOCT_pool_isEmpty(requestPool)) {	// ensure pool has contents
			continue;
		}
		requestArray = (eOCT_fieldRequest*)requestPool.array;

		// for each request
		for (int requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			request = &requestArray[requestCtr];
			if (iOCT_registry_findField(request->name, &match)) {	// if there is a match
				request->providerIndex_reg = match.providerIndex_reg;
				request->fieldOffset_reg = match.offset;
				request->global_reg = match.global_reg;
				request->fulfilled_reg = true;
				request->providerType_reg = match.providerType;
				if (request->cacheLocation) {
					*request->cacheLocation = *request;
				}
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

		// updateFx = system.updateFx;
		// if (updateFx) {
		// 	printf("Init system %s with UPDATE fx %p\n", system.name, updateFx);
		// 	iOCT_scheduler_addUpdateFx(updateFx);
		// }
		// else {
		// 	printf("System %s has no UPDATE fx\n", system.name);
		// }
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
	}
	printf("-----------------------\n\n");

}

#pragma endregion

#pragma region engine
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

eOCT_pool eOCT_generateFieldRequestPool(eOCT_fieldRequest* array, size_t count) {
	eOCT_pool pool = eOCT_pool_open(OCT_ID_NULL, count, sizeof(eOCT_fieldRequest));

	eOCT_fieldRequest* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

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
			iOCT_registry_registerComponent(component);
			iOCT_registry_registerFields(component->providedFields, systemID, eOCT_FIELDPROVIDER_COMPONENT, component->componentTypeIndex_reg, false);

			printf("%02"PRIu64".%02zu.--| %2cComponent %zu: %-15s\n", systemID, component->componentTypeIndex_reg, ' ', component->componentTypeIndex_reg, component->name);
		}
	}

	printf("\n");
	if (eOCT_pool_isEmpty(systemDescription->providedEvents)) {
		printf("No provided events\n");
	}
	else {
		eOCT_eventDescription* eventArray = (eOCT_eventDescription*)systemDescription->providedEvents.array;
		for (OCT_index eventCtr = 0; eventCtr < systemDescription->providedEvents.count; eventCtr++) {
			eOCT_eventDescription* event = &eventArray[eventCtr];
			iOCT_registry_registerEvent(event);
			iOCT_registry_registerFields(event->providedFields, systemID, eOCT_FIELDPROVIDER_EVENT, event->eventTypeIndex_reg, true);

			printf("%02"PRIu64".%02zu.--| %2cEvent %zu: %-15s\n", systemID, event->eventTypeIndex_reg, ' ', event->eventTypeIndex_reg, event->name);
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
			iOCT_registry_registerDataPool(dataPool);
			iOCT_registry_registerFields(dataPool->providedFields, systemID, eOCT_FIELDPROVIDER_DATAPOOL, dataPool->dataPoolTypeIndex_reg, dataPool->global);

			printf("%02"PRIu64".%02zu.--| %2cData Pool %zu: %-15s\n", systemID, dataPool->dataPoolTypeIndex_reg, ' ', dataPool->dataPoolTypeIndex_reg, dataPool->name);
		}
	}
	printf("--------------------------------\n\n");
}

#pragma endregion

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

	if (registryEntry->cacheLocation) {
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
static OCT_index iOCT_registry_registerFields(eOCT_pool providedFields, OCT_ID systemID, eOCT_fieldProvider providerType, OCT_index providerIndex, bool global) {
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
		printf("%02"PRIu64".%02zu.%02zu| %4cField: %-15s | ", systemID, providerIndex, fieldCtr, ' ', field->name);

		if (iOCT_registry_findField(field->name, NULL)) {	// check for duplicates
			printf("Failed: Field already exists\n");
			iOCT_registry_inst.success = false;
		}

		else {
			field->providerIndex_reg = providerIndex;
			field->global_reg = global;

			fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.fields, NULL);	// add field to the registry
			*fieldDestination = *field;
			printf("Success\n");
		}
	}

	printf("%13c Fields: %zu\n", ' ', providedFields.count);
	return fieldCtr;
}
#pragma endregion