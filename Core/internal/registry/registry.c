#include "types_internal.h"
#include "registry_internal.h"
#include "registry/registry.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "utilities/utilities_core.h"
#include "layout/systems.h"

#define iOCT_SYSTEM_INITIAL_CAPACITY 16

static bool iOCT_registry_fieldExists(const char* fieldName);

iOCT_registry iOCT_registry_instance = { 0 }; 

void iOCT_registry_init() {
	cOCT_pool systems = cOCT_pool_init(OCT_ID_CORE, iOCT_SYSTEM_INITIAL_CAPACITY, sizeof(cOCT_systemDescription));
	cOCT_pool fields = cOCT_pool_init(OCT_ID_CORE, iOCT_SYSTEM_INITIAL_CAPACITY, sizeof(cOCT_fieldDescription));
	iOCT_registry_instance.systems = systems;
	iOCT_registry_instance.fields = fields;
}

void cOCT_registry_registerSystem(cOCT_systemDescription systemDescription) {
	cOCT_systemDescription* destination = (cOCT_systemDescription*)cOCT_pool_addEntry(&iOCT_registry_instance.systems, NULL);
	*destination = systemDescription;

	int componentCtr = 0;
	int fieldCtr = 0;
	cOCT_componentDescription component;
	cOCT_fieldDescription field;
	cOCT_fieldDescription* fieldDestination;

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

			fieldDestination = (cOCT_fieldDescription*)cOCT_pool_addEntry(&iOCT_registry_instance.fields, NULL);	// add field to the registry
			*fieldDestination = field;
		}
	}
}

static bool iOCT_registry_fieldExists(const char* fieldName) {
	cOCT_pool* fields = &iOCT_registry_instance.fields;
	//printf("Number of fields in registry: %d\n", fields->count);
	cOCT_fieldDescription* fieldArray = (cOCT_fieldDescription*)fields->array;
	int fieldCtr = 0;

	for (fieldCtr = 0; fieldCtr < fields->count; fieldCtr++) {		// check every field in the registry
		if (strcmp(fieldArray[fieldCtr].name, fieldName) == 0) {
			return true;
		}
	}
	return false;
}