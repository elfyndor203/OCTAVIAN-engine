#pragma once
#include "testSystem.h"

#include "OCT_Core_eng.h"

typedef struct testPos {
	float posA;
	float posB;
	float posC;
	float posD;
	float posE;
	float posF;
	float posG;

} testPos;

typedef struct testVel {
	float velA;
	float velB;

} testVel;

void testDesc_register() {
	eOCT_fieldDescription posA = { "posA", OCT_FIELD_FLOAT, offsetof(testPos, posA) };
	eOCT_fieldDescription posB = { "posB", OCT_FIELD_FLOAT, offsetof(testPos, posB) };
	eOCT_fieldDescription posC = { "posC", OCT_FIELD_FLOAT, offsetof(testPos, posC) };
	eOCT_fieldDescription posD = { "posD", OCT_FIELD_FLOAT, offsetof(testPos, posD) };
	eOCT_fieldDescription posE = { "posE", OCT_FIELD_FLOAT, offsetof(testPos, posE) };
	eOCT_fieldDescription posF = { "posF", OCT_FIELD_FLOAT, offsetof(testPos, posF) };
	eOCT_fieldDescription posG = { "posG", OCT_FIELD_FLOAT, offsetof(testPos, posG) };
	eOCT_fieldDescription posFields[] = { posA, posB, posC, posD, posE, posF, posG };
	eOCT_componentDescription posComponent = {
		.name = "Position",
		.stride = sizeof(testPos),
		.providedFields = eOCT_generateFieldDescriptionPool(posFields, 7),
	};

	eOCT_fieldDescription velA = {"velA", OCT_FIELD_FLOAT, offsetof(testVel, velA) };
	eOCT_fieldDescription velB = {"velB", OCT_FIELD_FLOAT, offsetof(testVel, velB) };
	eOCT_fieldDescription velFields[] = { velA, velB };
	eOCT_componentDescription velComponent = {
		.name = "Velocity",
		.stride = sizeof(testVel),
		.providedFields = eOCT_generateFieldDescriptionPool(velFields, 2),
	};

	eOCT_componentDescription posComponent2 = {
		.name = "Position2",
		.stride = sizeof(testPos),
		.providedFields = eOCT_generateFieldDescriptionPool(posFields, 2),
	};

	eOCT_componentDescription components[] = { posComponent, velComponent, posComponent2 };
	eOCT_systemDescription systemA = {
		.name = "SystemA",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 3),
		.requestedFields = NULL,
	};

	eOCT_registry_registerSystem(systemA);
}
