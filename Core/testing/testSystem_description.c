#pragma once
#include "testSystem.h"

#include "OCT_Core_eng.h"

void testDesc_register() {
	eOCT_fieldDescription posA = {
	.name = "posA",
	.type = OCT_FIELD_FLOAT,
	.offset = offsetof(testPos, posA)
	};

	eOCT_fieldDescription posB = {
		.name = "posB",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testPos, posB)
	};

	eOCT_fieldDescription velA = {
		.name = "velA",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testVel, velA)
	};

	eOCT_fieldDescription velB = {
		.name = "velB",
		.type = OCT_FIELD_FLOAT,
		.offset = offsetof(testVel, velB)
	};

	eOCT_fieldDescription posFields[] = { posA, posB };
	eOCT_fieldDescription velFields[] = { velA, velB };

	eOCT_componentDescription posComponent = {
		.name = "Position",
		.stride = sizeof(testPos),
		.providedFields = posFields,
		.providedFieldsCount = 2
	};
	eOCT_componentDescription velComponent = {
		.name = "Velocity",
		.stride = sizeof(testPos),
		.providedFields = velFields,
		.providedFieldsCount = 2
	};

	eOCT_componentDescription posComponent2 = {
		.name = "Position2",
		.stride = sizeof(testPos),
		.providedFields = posFields,
		.providedFieldsCount = 2
	};

	eOCT_componentDescription components[] = { posComponent, velComponent, posComponent2 };

	eOCT_systemDescription systemA = {
		.name = "SystemA",
		.providedComponents = components,
		.providedComponentsCount = 3,
		.requestedFields = NULL,
		.requestedFieldsCount = 0
	};

	eOCT_registry_registerSystem(systemA);
}
