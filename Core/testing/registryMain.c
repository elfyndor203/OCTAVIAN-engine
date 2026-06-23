#include "OCT_Core_core.h"
#include "registry/registry_internal.h"

typedef struct testPos {
	float posA;
	float posB;
} testPos;

typedef struct testVel {
	float velA;
	float velB;

} testVel;;



int main() {

	cOCT_fieldDescription posA = {
	.name = "posA",
	.type = OCT_FIELD_FLOAT,
	.offset = offsetof(testPos, posA)
	};

	cOCT_fieldDescription posB = {
		.name = "posB",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testPos, posB)
	};

	cOCT_fieldDescription velA = {
		.name = "velA",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testVel, velA)
	};

	cOCT_fieldDescription velB = {
		.name = "velB",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testVel, velB)
	};

	cOCT_fieldDescription posFields[] = { posA, posB };
	cOCT_fieldDescription velFields[] = { velA, velB };

	cOCT_componentDescription posComponent = {
		.name = "Position",
		.stride = sizeof(testPos),
		.providedFields = posFields,
		.providedFieldsCount = 2
	};
	cOCT_componentDescription velComponent = {
		.name = "Velocity",
		.stride = sizeof(testPos),
		.providedFields = velFields,
		.providedFieldsCount = 2
	};

	cOCT_componentDescription posComponent2 = {
		.name = "Position2",
		.stride = sizeof(testPos),	
		.providedFields = posFields,
		.providedFieldsCount = 2
	};

	cOCT_componentDescription components[] = { posComponent, velComponent, posComponent2 };

	cOCT_systemDescription systemA = {
		.name = "SystemA",
		.providedComponents = components,
		.providedComponentsCount = 3,
		.requestedFields = NULL,
		.requestedFieldsCount = 0
	};

	iOCT_registry_init();
	cOCT_registry_registerSystem(systemA);
	return 0;
}