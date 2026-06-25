/* ============================================================
   System description variations for the modular ECS registry.

   Four systems are defined:

   1. testDesc_register        - baseline, valid (your original,
								  with the stride/pool bugs fixed)
   2. systemB_register         - valid, different shapes/fields
   3. systemC_register_fails   - INVALID: two components inside
								  the SAME system both provide
								  "rotZ", so registration should
								  be rejected by the registry
   4. systemD_register_fails   - INVALID: two otherwise-fine
								  systems are registered back to
								  back, but they share a field
								  name ("health") on components
								  with different strides/owners,
								  so the cross-system check should
								  reject the second registration
   ============================================================ */

#include <stddef.h>
#include "OCT_Core_eng.h"
#include <stdio.h>
#include <inttypes.h>

typedef struct systems {
	eOCT_systemDescription sysList[5];
} systems;

systems allSystems = { 0 };

/* ------------------------------------------------------------
   1. Baseline system - valid
   ------------------------------------------------------------ */

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
	eOCT_fieldDescription posA = { "posA", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posA) };
	eOCT_fieldDescription posB = { "posB", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posB) };
	eOCT_fieldDescription posC = { "posC", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posC) };
	eOCT_fieldDescription posD = { "posD", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posD) };
	eOCT_fieldDescription posE = { "posE", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posE) };
	eOCT_fieldDescription posF = { "posF", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posF) };
	eOCT_fieldDescription posG = { "posG", eOCT_FIELDTYPE_FLOAT32, offsetof(testPos, posG) };
	eOCT_fieldDescription posFields[] = { posA, posB, posC, posD, posE, posF, posG };

	eOCT_fieldDescription velA = { "velA", eOCT_FIELDTYPE_FLOAT32, offsetof(testVel, velA) };
	eOCT_fieldDescription velB = { "velB", eOCT_FIELDTYPE_FLOAT32, offsetof(testVel, velB) };
	eOCT_fieldDescription velFields[] = { velA, velB };

	eOCT_componentDescription posComponent = { "Position", sizeof(testPos), eOCT_generateFieldDescriptionPool(posFields, 7) };
	eOCT_componentDescription velComponent = { "Velocity", sizeof(testVel), eOCT_generateFieldDescriptionPool(velFields, 2) };
	eOCT_componentDescription components[] = { posComponent, velComponent };

	eOCT_fieldRequest rotX = { "rotX", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest rotY = { "rotY", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest rotW = { "rotW", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest rotZ = { "rotZ", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest requests[] = { rotX, rotY, rotW, rotZ };

	eOCT_systemDescription systemA = {
		.name = "SystemA",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = eOCT_generateFieldRequestPool(requests, 4)
	};

	allSystems.sysList[0] = systemA;
	eOCT_registry_registerSystem(&allSystems.sysList[0]);
}


/* ------------------------------------------------------------
   2. SystemB - valid, different shapes

   requestedFields mix: "healthCurrent" and "healthMax" are real
   fields that SystemB itself provides (requesting your own
   provided fields should be harmless/valid), "posA" is a real
   field provided by SystemA, and "manaPool" does NOT exist
   anywhere in the registry - that one should cause the registry
   to reject this request list (or otherwise flag it) since no
   system provides it.
   ------------------------------------------------------------ */

typedef struct testRot {
	float rotX;
	float rotY;
	float rotW;
	float rotZ;
	float angVelX;
	float angVelY;
	float angVelZ;
	float angAccelX;
	float angAccelY;
	float angAccelZ;
	float pivotX;
	float pivotY;
	float pivotZ;
	float eulerPitch;
	float eulerYaw;
	float eulerRoll;
	float prevRotX;
	float prevRotY;
	float prevRotZ;
	float prevRotW;
	float dampening;
	float lockThreshold;
	float interpAlpha;
} testRot;

typedef struct testHealth {
	float current;
	float max;
} testHealth;

void systemB_register() {
	eOCT_fieldDescription rotX = { "rotX", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, rotX) };
	eOCT_fieldDescription rotY = { "rotY", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, rotY) };
	eOCT_fieldDescription rotW = { "rotW", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, rotW) };
	eOCT_fieldDescription rotZ = { "rotZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, rotZ) };
	eOCT_fieldDescription angVelX = { "angVelX", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angVelX) };
	eOCT_fieldDescription angVelY = { "angVelY", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angVelY) };
	eOCT_fieldDescription angVelZ = { "angVelZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angVelZ) };
	eOCT_fieldDescription angAccelX = { "angAccelX", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angAccelX) };
	eOCT_fieldDescription angAccelY = { "angAccelY", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angAccelY) };
	eOCT_fieldDescription angAccelZ = { "angAccelZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, angAccelZ) };
	eOCT_fieldDescription pivotX = { "pivotX", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, pivotX) };
	eOCT_fieldDescription pivotY = { "pivotY", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, pivotY) };
	eOCT_fieldDescription pivotZ = { "pivotZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, pivotZ) };
	eOCT_fieldDescription eulerPitch = { "eulerPitch", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, eulerPitch) };
	eOCT_fieldDescription eulerYaw = { "eulerYaw", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, eulerYaw) };
	eOCT_fieldDescription eulerRoll = { "eulerRoll", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, eulerRoll) };
	eOCT_fieldDescription prevRotX = { "prevRotX", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, prevRotX) };
	eOCT_fieldDescription prevRotY = { "prevRotY", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, prevRotY) };
	eOCT_fieldDescription prevRotZ = { "prevRotZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, prevRotZ) };
	eOCT_fieldDescription prevRotW = { "prevRotW", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, prevRotW) };
	eOCT_fieldDescription dampening = { "dampening", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, dampening) };
	eOCT_fieldDescription lockThreshold = { "lockThreshold", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, lockThreshold) };
	eOCT_fieldDescription interpAlpha = { "interpAlpha", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot, interpAlpha) };

	eOCT_fieldDescription rotFields[] = {
		rotX, rotY, rotW, rotZ,
		angVelX, angVelY, angVelZ,
		angAccelX, angAccelY, angAccelZ,
		pivotX, pivotY, pivotZ,
		eulerPitch, eulerYaw, eulerRoll,
		prevRotX, prevRotY, prevRotZ, prevRotW,
		dampening, lockThreshold, interpAlpha
	};

	eOCT_componentDescription rotComponent = {
		.name = "Rotation",
		.stride = sizeof(testRot),
		.providedFields = eOCT_generateFieldDescriptionPool(rotFields, 23),
	};

	eOCT_fieldDescription healthCurrent = { "healthCurrent", eOCT_FIELDTYPE_FLOAT32, offsetof(testHealth, current) };
	eOCT_fieldDescription healthMax = { "healthMax", eOCT_FIELDTYPE_FLOAT32, offsetof(testHealth, max) };
	eOCT_fieldDescription healthFields[] = { healthCurrent, healthMax };

	eOCT_componentDescription healthComponent = {
		.name = "Health",
		.stride = sizeof(testHealth),
		.providedFields = eOCT_generateFieldDescriptionPool(healthFields, 2),
	};

	eOCT_componentDescription components[] = { rotComponent, healthComponent };

	eOCT_fieldRequest reqHealthCurrent = { "healthCurrent", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqHealthMax = { "healthMax", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqPosA = { "posA", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqManaPool = { "manaPool", eOCT_FIELDTYPE_FLOAT32 }; /* does not exist anywhere - should error */
	eOCT_fieldRequest requestsB[] = { reqHealthCurrent, reqHealthMax, reqPosA, reqManaPool };

	eOCT_systemDescription systemB = {
		.name = "SystemB",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = eOCT_generateFieldRequestPool(requestsB, 4),
	};

	allSystems.sysList[1] = systemB;
	eOCT_registry_registerSystem(&allSystems.sysList[1]);
}


/* ------------------------------------------------------------
   3. SystemC - INVALID: duplicate field WITHIN the same system

   Two different components ("Rotation2" and "RotationAlt") both
   declare a field literally named "rotZ". They're separate
   structs with separate storage, but the registry should reject
   this because the same system can't provide two fields with the
   same name.

   requestedFields here are "rotSpeed" (real, provided by this
   same system's Rotation2 component) and "velA" (real, provided
   by SystemA's Velocity component) plus "stamina" which doesn't
   exist anywhere - included for completeness, but the duplicate
   "rotZ" provided-field collision is expected to be caught first.
   ------------------------------------------------------------ */

typedef struct testRot2 {
	float rotZ;
	float rotSpeed;
} testRot2;

typedef struct testRotAlt {
	float rotZ;       /* duplicate name, different struct/offset */
	float rotDamping;
} testRotAlt;

void systemC_register_fails() {
	eOCT_fieldDescription rotZ_a = { "rotZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot2, rotZ) };
	eOCT_fieldDescription rotSpeed = { "rotSpeed", eOCT_FIELDTYPE_FLOAT32, offsetof(testRot2, rotSpeed) };
	eOCT_fieldDescription rot2Fields[] = { rotZ_a, rotSpeed };

	eOCT_componentDescription rotation2Component = {
		.name = "Rotation2",
		.stride = sizeof(testRot2),
		.providedFields = eOCT_generateFieldDescriptionPool(rot2Fields, 2),
	};

	eOCT_fieldDescription rotZ_b = { "rotZ", eOCT_FIELDTYPE_FLOAT32, offsetof(testRotAlt, rotZ) }; /* collides with rotZ_a */
	eOCT_fieldDescription rotDamping = { "rotDamping", eOCT_FIELDTYPE_FLOAT32, offsetof(testRotAlt, rotDamping) };
	eOCT_fieldDescription rotAltFields[] = { rotZ_b, rotDamping };

	eOCT_componentDescription rotationAltComponent = {
		.name = "RotationAlt",
		.stride = sizeof(testRotAlt),
		.providedFields = eOCT_generateFieldDescriptionPool(rotAltFields, 2),
	};

	eOCT_componentDescription components[] = { rotation2Component, rotationAltComponent };

	eOCT_fieldRequest reqRotSpeed = { "rotSpeed", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqVelA = { "velA", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqStamina = { "stamina", eOCT_FIELDTYPE_FLOAT32 }; /* does not exist anywhere - should error */
	eOCT_fieldRequest requestsC[] = { reqRotSpeed, reqVelA, reqStamina };

	eOCT_systemDescription systemC = {
		.name = "SystemC",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = eOCT_generateFieldRequestPool(requestsC, 3),
	};

	/* EXPECTED: eOCT_registry_registerSystem should reject this,
	   since "rotZ" is provided twice within SystemC's own
	   component set. */
	allSystems.sysList[2] = systemC;
	eOCT_registry_registerSystem(&allSystems.sysList[2]);
}


/* ------------------------------------------------------------
   4. SystemD - INVALID: duplicate field ACROSS systems

   SystemD1 registers cleanly on its own. SystemD2 also looks
   fine in isolation, but it reuses the field name "health" that
   SystemD1 already registered under a totally different
   component/struct. If the registry tracks field names globally
   (not just per-system), registering SystemD2 after SystemD1
   should fail.

   SystemD1's requests ("regenRate" - its own field, "posB" -
   real, from SystemA) are valid. SystemD2's requests include
   "armor" (its own real field) and "luckStat" (does not exist
   anywhere - should error) in addition to "health" itself, which
   becomes ambiguous/invalid once SystemD1 has already claimed it.
   ------------------------------------------------------------ */

typedef struct testShield {
	float health;     /* same field name as testCreatureHealth.health below */
	float regenRate;
} testShield;

typedef struct testCreatureHealth {
	float health;     /* collides with testShield.health */
	float armor;
} testCreatureHealth;

void systemD_register_fails() {
	/* --- SystemD1: registers fine on its own --- */
	eOCT_fieldDescription shieldHealth = { "health", eOCT_FIELDTYPE_FLOAT32, offsetof(testShield, health) };
	eOCT_fieldDescription regenRate = { "regenRate", eOCT_FIELDTYPE_FLOAT32, offsetof(testShield, regenRate) };
	eOCT_fieldDescription shieldFields[] = { shieldHealth, regenRate };

	eOCT_componentDescription shieldComponent = {
		.name = "Shield",
		.stride = sizeof(testShield),
		.providedFields = eOCT_generateFieldDescriptionPool(shieldFields, 2),
	};

	eOCT_componentDescription d1Components[] = { shieldComponent };

	eOCT_fieldRequest reqRegenRate = { "regenRate", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqPosB = { "posB", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest requestsD1[] = { reqRegenRate, reqPosB };

	eOCT_systemDescription systemD1 = {
		.name = "SystemD1",
		.providedComponents = eOCT_generateComponentDescriptionPool(d1Components, 1),
		.requestedFields = eOCT_generateFieldRequestPool(requestsD1, 2),
	};

	/* --- SystemD2: reuses "health", should fail at registration --- */
	eOCT_fieldDescription creatureHealth = { "health", eOCT_FIELDTYPE_FLOAT32, offsetof(testCreatureHealth, health) };
	eOCT_fieldDescription armor = { "armor", eOCT_FIELDTYPE_FLOAT32, offsetof(testCreatureHealth, armor) };
	eOCT_fieldDescription creatureFields[] = { creatureHealth, armor };

	eOCT_componentDescription creatureHealthComponent = {
		.name = "CreatureHealth",
		.stride = sizeof(testCreatureHealth),
		.providedFields = eOCT_generateFieldDescriptionPool(creatureFields, 2),
	};

	eOCT_componentDescription d2Components[] = { creatureHealthComponent };

	eOCT_fieldRequest reqArmor = { "armor", eOCT_FIELDTYPE_FLOAT32 };
	eOCT_fieldRequest reqLuckStat = { "luckStat", eOCT_FIELDTYPE_FLOAT32 }; /* does not exist anywhere - should error */
	eOCT_fieldRequest requestsD2[] = { reqArmor, reqLuckStat };

	eOCT_systemDescription systemD2 = {
		.name = "SystemD2",
		.providedComponents = eOCT_generateComponentDescriptionPool(d2Components, 1),
		.requestedFields = eOCT_generateFieldRequestPool(requestsD2, 2),
	};

	/* EXPECTED: eOCT_registry_registerSystem should reject this,
	   since "health" was already claimed by SystemD1's Shield
	   component. */

	allSystems.sysList[3] = systemD1;
	allSystems.sysList[4] = systemD2;
	eOCT_registry_registerSystem(&allSystems.sysList[3]);
	eOCT_registry_registerSystem(&allSystems.sysList[4]);
}

