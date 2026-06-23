#include "types_int.h"
#include "registry_int.h"
#include "registry/registry_eng.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "utilities/utilities_eng.h"
#include "layout/systems.h"

#define iOCT_SYSTEM_INITIAL_CAPACITY 16

static bool iOCT_registry_fieldExists(const char* fieldName);

iOCT_registry iOCT_registry_inst = { 0 }; 

void iOCT_registry_init() {
	eOCT_pool systems = eOCT_pool_init(OCT_ID_REGISTRY, iOCT_SYSTEM_INITIAL_CAPACITY, sizeof(eOCT_systemDescription));
	eOCT_pool fields = eOCT_pool_init(OCT_ID_REGISTRY, iOCT_SYSTEM_INITIAL_CAPACITY, sizeof(eOCT_fieldDescription));
	iOCT_registry_inst.systems = systems;
	iOCT_registry_inst.fields = fields;
	iOCT_registry_inst.componentCount = 0;

	printf("Registry initialized:\n");
	printf("  Systems capacity: %zu\n", iOCT_registry_inst.systems.capacity);
	printf("  Fields capacity: %zu\n", iOCT_registry_inst.fields.capacity);
}

void eOCT_registry_registerSystem(eOCT_systemDescription systemDescription) {
	eOCT_systemDescription* destination = (eOCT_systemDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.systems, NULL);
	*destination = systemDescription;

	int componentCtr = 0;
	int fieldCtr = 0;
	eOCT_componentDescription component;
	eOCT_fieldDescription field;
	eOCT_fieldDescription* fieldDestination;

	printf("\nRegistering system '%s' with %d components.\n", systemDescription.name, systemDescription.providedComponentsCount);
	for (componentCtr = 0; componentCtr < systemDescription.providedComponentsCount; componentCtr++) {		// ensures that provided fields do not already exist, then adds them to the field pool
		component = systemDescription.providedComponents[componentCtr];
		printf("%2cRegistering component '%s' with %d fields.\n", ' ', component.name, component.providedFieldsCount);

		for (fieldCtr = 0; fieldCtr < component.providedFieldsCount; fieldCtr++) {
			field = component.providedFields[fieldCtr];

			if (iOCT_registry_fieldExists(field.name)) {	// check for duplicates
				printf("\nError: Field '%s' already exists in the registry.\n", field.name);
				return;
			}
			else {
				printf("%4cRegistering field '%s' in the registry.\n", ' ', field.name);
			}

			fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.fields, NULL);	// add field to the registry
			*fieldDestination = field;
		}
	}

	iOCT_registry_inst.componentCount += systemDescription.providedComponentsCount;
	printf("	Total components in registry: %u\n", iOCT_registry_inst.componentCount);
}

static bool iOCT_registry_fieldExists(const char* fieldName) {
	eOCT_pool* fields = &iOCT_registry_inst.fields;
	//printf("Number of fields in registry: %d\n", fields->count);
	eOCT_fieldDescription* fieldArray = (eOCT_fieldDescription*)fields->array;
	int fieldCtr = 0;

	for (fieldCtr = 0; fieldCtr < fields->count; fieldCtr++) {		// check every field in the registry
		if (strcmp(fieldArray[fieldCtr].name, fieldName) == 0) {
			return true;
		}
	}
	return false;
}