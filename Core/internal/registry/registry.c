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

static bool iOCT_registry_findField(const char* fieldName, eOCT_fieldDescription* fieldOut);

iOCT_registry iOCT_registry_inst = { 0 }; 

#pragma region internal
void init_OCT_registry_init() {
	eOCT_pool systems = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_systemDescription*)); // store system pointers
	eOCT_pool fields = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_fieldDescription));
	eOCT_pool components = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_componentDescription));
	iOCT_registry_inst.systems = systems;
	iOCT_registry_inst.components = components;
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

		for (int requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			request = &requestArray[requestCtr];
			if (iOCT_registry_findField(request->name, &match)) {	// if there is a match
				request->componentTypeIndex_reg = match.componentTypeIndex_reg;
				request->fieldOffset_reg = match.offset;
				request->fulfilled_reg = true;
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
	printf("Systems: (1 & 2 reserved)\n");
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
	printf("ECS Component Count: %zu\n", iOCT_ECS_inst.componentTypeCount);
	printf("ECS Entity Size: %zu\n", iOCT_ECS_inst.entitySize);

	printf("\nFields requested:\n");
	for (systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		system = *systemArray[systemCtr];
		requestPool = system.requestedFields;
		requestArray = (eOCT_fieldRequest*)requestPool.array;
		for (requestCtr = 0; requestCtr < requestPool.count; requestCtr++) {
			request = requestArray[requestCtr];
			if (request.fulfilled_reg) {
				printf("%02zu[%02zu]", request.componentTypeIndex_reg, request.fieldOffset_reg);
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
	eOCT_pool pool = eOCT_pool_init(OCT_ID_NULL, count, sizeof(eOCT_fieldDescription));
	eOCT_fieldDescription* destination;
	for (OCT_index ctr = 0; ctr < count; ctr++) {
		destination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, size_t count) {
	eOCT_pool pool = eOCT_pool_init(OCT_ID_NULL, count, sizeof(eOCT_componentDescription));

	eOCT_componentDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

eOCT_pool eOCT_generateFieldRequestPool(eOCT_fieldRequest* array, size_t count) {
	eOCT_pool pool = eOCT_pool_init(OCT_ID_NULL, count, sizeof(eOCT_fieldRequest));

	eOCT_fieldRequest* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

void eOCT_registry_registerSystem(eOCT_systemDescription* systemDescription) {
	OCT_ID systemID = iOCT_registry_inst.systems.count + OCT_ID_SYSTEM_START;

	eOCT_systemDescription** destination = (eOCT_systemDescription**)eOCT_pool_addEntry(&iOCT_registry_inst.systems, NULL);	// addEntry after so the ID starts at 3 instead of 3 + 1
	*destination = systemDescription;

	int componentCtr = 0;
	int fieldCtr = 0;

	if (eOCT_pool_isEmpty(systemDescription->providedComponents)) {		// requests handed separately later, so registration ends
		printf("No provided components\n");
		printf("--------------------------------\n\n");
		return;
	}
	eOCT_componentDescription* componentArray = (eOCT_componentDescription*)systemDescription->providedComponents.array;
	eOCT_componentDescription* component;				// pointer to return index to
	eOCT_componentDescription* componentDestination;
	eOCT_fieldDescription* fieldArray;
	eOCT_fieldDescription* field;						// return component index to fields too for linking
	eOCT_fieldDescription* fieldDestination;

	systemDescription->systemID_reg = systemID;
	printf("\n--------------------------------\n");
	printf("%02"PRIu64".--.--| System '%s':\n", systemDescription->systemID_reg, systemDescription->name);
	//printf("%11c Components: %zu\n", ' ', systemDescription->providedComponents.count);
	for (componentCtr = 0; componentCtr < systemDescription->providedComponents.count; componentCtr++) {		// ensures that provided fields do not already exist, then adds them to the field pool
		component = &componentArray[componentCtr];
		componentDestination = (eOCT_componentDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.components, NULL);
		*componentDestination = *component;
		component->componentTypeIndex_reg = iOCT_ECS_addComponentType(*component); // tells the system the index of the component's pool

		printf("%02"PRIu64".%02zu.--| %2cComponent %zu: %-15s\n", systemID, component->componentTypeIndex_reg, ' ', component->componentTypeIndex_reg, component->name);

		if (eOCT_pool_isEmpty(systemDescription->providedComponents)) {
			printf("Component has no public fields\n");
			continue;
		}
		fieldArray = (eOCT_fieldDescription*)component->providedFields.array;
		for (fieldCtr = 0; fieldCtr < component->providedFields.count; fieldCtr++) {
			field = &fieldArray[fieldCtr];
			printf("%02"PRIu64".%02zu.%02d| %4cField: %-15s | ", systemID, component->componentTypeIndex_reg, fieldCtr, ' ', field->name);

			if (iOCT_registry_findField(field->name, NULL)) {	// check for duplicates
				printf("Failed: Field already exists\n");
				iOCT_registry_inst.success = false;
			}

			else {
				field->componentTypeIndex_reg = component->componentTypeIndex_reg;

				fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.fields, NULL);	// add field to the registry
				*fieldDestination = *field;
				printf("Success\n");
			}
		}
		printf("%13c Fields: %zu\n", ' ', component->providedFields.count);
		if (component->cacheLocation) {
			*component->cacheLocation = *component;	// cache the component optionally
		}
	}
	printf("--------------------------------\n\n");
}

#pragma endregion

#pragma region static
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
#pragma endregion