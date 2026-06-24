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

static bool iOCT_registry_fieldExists(const char* fieldName);

iOCT_registry iOCT_registry_inst = { 0 }; 

void iOCT_registry_init() {
	eOCT_pool systems = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_systemDescription));
	eOCT_pool fields = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_fieldDescription));
	eOCT_pool components = eOCT_pool_init(OCT_ID_REGISTRY, eOCT_POOLSIZE_DEFAULT, sizeof(eOCT_componentDescription));
	iOCT_registry_inst.systems = systems;
	iOCT_registry_inst.components = components;
	iOCT_registry_inst.fields = fields;
	iOCT_registry_inst.componentCount = 0;

	printf("\nRegistry initialized:\n");
	printf("  | Systems capacity: %zu\n", iOCT_registry_inst.systems.capacity);
	printf("  | Components capacity: %zu\n", iOCT_registry_inst.components.capacity);
	printf("  | Fields capacity: %zu\n", iOCT_registry_inst.fields.capacity);
	printf("\n");
}

void eOCT_registry_registerSystem(eOCT_systemDescription* systemDescription) {
	eOCT_systemDescription* destination = (eOCT_systemDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.systems, NULL);
	*destination = *systemDescription;

	OCT_ID systemID = iOCT_registry_inst.systemCount + 1; // start at 1 because 0 is null
	int componentCtr = 0;
	int fieldCtr = 0;
	eOCT_componentDescription* componentArray = (eOCT_componentDescription*)systemDescription->providedComponents.array;
	eOCT_componentDescription component;
	eOCT_componentDescription* componentDestination;
	eOCT_fieldDescription* fieldArray;
	eOCT_fieldDescription field;
	eOCT_fieldDescription* fieldDestination;

	systemDescription->systemID_reg = systemID;
	printf("\n[--------------------------------\n");
	printf("%02"PRIu64".%02d.%02d| System '%s' summary:\n", systemDescription->systemID_reg, 0, 0, systemDescription->name);
	printf("%11c Components: %zu\n", ' ', systemDescription->providedComponents.count);
	for (componentCtr = 0; componentCtr < systemDescription->providedComponents.count; componentCtr++) {		// ensures that provided fields do not already exist, then adds them to the field pool
		component = componentArray[componentCtr];
		componentDestination = (eOCT_componentDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.components, NULL);
		*componentDestination = component;
		printf("%02"PRIu64".%02d.%02d| %2cComponent: %-15s\n", systemID, componentCtr + 1, 0, ' ', component.name);

		fieldArray = (eOCT_fieldDescription*)component.providedFields.array;
		for (fieldCtr = 0; fieldCtr < component.providedFields.count; fieldCtr++) {
			field = fieldArray[fieldCtr];
			printf("%02"PRIu64".%02d.%02d| %4cField: %-15s | ", systemID, componentCtr + 1, fieldCtr + 1, ' ', field.name);

			if (iOCT_registry_fieldExists(field.name)) {	// check for duplicates
				printf("Failed: Field already exists");
			}
			else {
				fieldDestination = (eOCT_fieldDescription*)eOCT_pool_addEntry(&iOCT_registry_inst.fields, NULL);	// add field to the registry
				*fieldDestination = field;
				printf("Success");
			}
			printf("\n");
			iOCT_registry_inst.fieldCount++;
		}
		printf("%13c Fields: %zu\n", ' ', component.providedFields.count);
		iOCT_registry_inst.componentCount++;

		component.componentIndex_reg = iOCT_ECS_addComponent(component); // tells the system the index of the component's pool
	}
	printf("--------------------------------]\n\n");
	iOCT_registry_inst.systemCount++;
}

void iOCT_registry_summary() {
	printf("Total systems in registry: %u\n", iOCT_registry_inst.systemCount);
	printf("Total components in registry: %u\n", iOCT_registry_inst.componentCount);
	printf("Total fields in registry: %u\n", iOCT_registry_inst.fieldCount);
	printf("\n");
}

void eOCT_registry_allocateComponents() {
	eOCT_systemDescription* systemArray = (eOCT_systemDescription*)iOCT_registry_inst.systems.array;
	eOCT_systemDescription* system;
	eOCT_componentDescription* componentArray;
	eOCT_componentDescription* component;

	//for (int systemCtr = 0; systemCtr < iOCT_registry_inst.systemCount; systemCtr++) {
	//	system = &systemArray[systemCtr];
	//	system->systemID_reg = systemCtr;
	//	
	//	componentArray = system->providedComponents
	//}
}

eOCT_pool eOCT_generateFieldDescriptionPool(eOCT_fieldDescription* array, unsigned int count) {
	eOCT_pool pool = eOCT_pool_init(OCT_ID_NULL, count, sizeof(eOCT_fieldDescription));

	eOCT_fieldDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
}

eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, unsigned int count) {
	eOCT_pool pool = eOCT_pool_init(OCT_ID_NULL, count, sizeof(eOCT_componentDescription));

	eOCT_componentDescription* destination;
	for (int ctr = 0; ctr < count; ctr++) {
		destination = eOCT_pool_addEntry(&pool, NULL);
		*destination = array[ctr];
	}
	return pool;
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