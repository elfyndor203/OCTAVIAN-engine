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

	eOCT_fieldDescription velA = { "velA", OCT_FIELD_FLOAT, offsetof(testVel, velA) };
	eOCT_fieldDescription velB = { "velB", OCT_FIELD_FLOAT, offsetof(testVel, velB) };
	eOCT_fieldDescription velFields[] = { velA, velB };

	eOCT_componentDescription velComponent = {
		.name = "Velocity",
		.stride = sizeof(testVel),   /* fixed: was sizeof(testPos) */
		.providedFields = eOCT_generateFieldDescriptionPool(velFields, 2),
	};

	eOCT_componentDescription components[] = { posComponent, velComponent };

	eOCT_systemDescription systemA = {
		.name = "SystemA",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = NULL,
	};

	eOCT_registry_registerSystem(systemA);
}


/* ------------------------------------------------------------
   2. SystemB - valid, different shapes
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
	eOCT_fieldDescription rotX = { "rotX", OCT_FIELD_FLOAT, offsetof(testRot, rotX) };
	eOCT_fieldDescription rotY = { "rotY", OCT_FIELD_FLOAT, offsetof(testRot, rotY) };
	eOCT_fieldDescription rotW = { "rotW", OCT_FIELD_FLOAT, offsetof(testRot, rotW) };
	eOCT_fieldDescription rotZ = { "rotZ", OCT_FIELD_FLOAT, offsetof(testRot, rotZ) };
	eOCT_fieldDescription angVelX = { "angVelX", OCT_FIELD_FLOAT, offsetof(testRot, angVelX) };
	eOCT_fieldDescription angVelY = { "angVelY", OCT_FIELD_FLOAT, offsetof(testRot, angVelY) };
	eOCT_fieldDescription angVelZ = { "angVelZ", OCT_FIELD_FLOAT, offsetof(testRot, angVelZ) };
	eOCT_fieldDescription angAccelX = { "angAccelX", OCT_FIELD_FLOAT, offsetof(testRot, angAccelX) };
	eOCT_fieldDescription angAccelY = { "angAccelY", OCT_FIELD_FLOAT, offsetof(testRot, angAccelY) };
	eOCT_fieldDescription angAccelZ = { "angAccelZ", OCT_FIELD_FLOAT, offsetof(testRot, angAccelZ) };
	eOCT_fieldDescription pivotX = { "pivotX", OCT_FIELD_FLOAT, offsetof(testRot, pivotX) };
	eOCT_fieldDescription pivotY = { "pivotY", OCT_FIELD_FLOAT, offsetof(testRot, pivotY) };
	eOCT_fieldDescription pivotZ = { "pivotZ", OCT_FIELD_FLOAT, offsetof(testRot, pivotZ) };
	eOCT_fieldDescription eulerPitch = { "eulerPitch", OCT_FIELD_FLOAT, offsetof(testRot, eulerPitch) };
	eOCT_fieldDescription eulerYaw = { "eulerYaw", OCT_FIELD_FLOAT, offsetof(testRot, eulerYaw) };
	eOCT_fieldDescription eulerRoll = { "eulerRoll", OCT_FIELD_FLOAT, offsetof(testRot, eulerRoll) };
	eOCT_fieldDescription prevRotX = { "prevRotX", OCT_FIELD_FLOAT, offsetof(testRot, prevRotX) };
	eOCT_fieldDescription prevRotY = { "prevRotY", OCT_FIELD_FLOAT, offsetof(testRot, prevRotY) };
	eOCT_fieldDescription prevRotZ = { "prevRotZ", OCT_FIELD_FLOAT, offsetof(testRot, prevRotZ) };
	eOCT_fieldDescription prevRotW = { "prevRotW", OCT_FIELD_FLOAT, offsetof(testRot, prevRotW) };
	eOCT_fieldDescription dampening = { "dampening", OCT_FIELD_FLOAT, offsetof(testRot, dampening) };
	eOCT_fieldDescription lockThreshold = { "lockThreshold", OCT_FIELD_FLOAT, offsetof(testRot, lockThreshold) };
	eOCT_fieldDescription interpAlpha = { "interpAlpha", OCT_FIELD_FLOAT, offsetof(testRot, interpAlpha) };

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

	eOCT_fieldDescription healthCurrent = { "healthCurrent", OCT_FIELD_FLOAT, offsetof(testHealth, current) };
	eOCT_fieldDescription healthMax = { "healthMax", OCT_FIELD_FLOAT, offsetof(testHealth, max) };
	eOCT_fieldDescription healthFields[] = { healthCurrent, healthMax };

	eOCT_componentDescription healthComponent = {
		.name = "Health",
		.stride = sizeof(testHealth),
		.providedFields = eOCT_generateFieldDescriptionPool(healthFields, 2),
	};

	eOCT_componentDescription components[] = { rotComponent, healthComponent };

	eOCT_systemDescription systemB = {
		.name = "SystemB",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = NULL,
	};

	eOCT_registry_registerSystem(systemB);
}


/* ------------------------------------------------------------
   3. SystemC - INVALID: duplicate field WITHIN the same system

   Two different components ("Rotation2" and "RotationAlt") both
   declare a field literally named "rotZ". They're separate
   structs with separate storage, but the registry should reject
   this because the same system can't provide two fields with the
   same name.
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
	eOCT_fieldDescription rotZ_a = { "rotZ", OCT_FIELD_FLOAT, offsetof(testRot2, rotZ) };
	eOCT_fieldDescription rotSpeed = { "rotSpeed", OCT_FIELD_FLOAT, offsetof(testRot2, rotSpeed) };
	eOCT_fieldDescription rot2Fields[] = { rotZ_a, rotSpeed };

	eOCT_componentDescription rotation2Component = {
		.name = "Rotation2",
		.stride = sizeof(testRot2),
		.providedFields = eOCT_generateFieldDescriptionPool(rot2Fields, 2),
	};

	eOCT_fieldDescription rotZ_b = { "rotZ", OCT_FIELD_FLOAT, offsetof(testRotAlt, rotZ) }; /* collides with rotZ_a */
	eOCT_fieldDescription rotDamping = { "rotDamping", OCT_FIELD_FLOAT, offsetof(testRotAlt, rotDamping) };
	eOCT_fieldDescription rotAltFields[] = { rotZ_b, rotDamping };

	eOCT_componentDescription rotationAltComponent = {
		.name = "RotationAlt",
		.stride = sizeof(testRotAlt),
		.providedFields = eOCT_generateFieldDescriptionPool(rotAltFields, 2),
	};

	eOCT_componentDescription components[] = { rotation2Component, rotationAltComponent };

	eOCT_systemDescription systemC = {
		.name = "SystemC",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
		.requestedFields = NULL,
	};

	/* EXPECTED: eOCT_registry_registerSystem should reject this,
	   since "rotZ" is provided twice within SystemC's own
	   component set. */
	eOCT_registry_registerSystem(systemC);
}


/* ------------------------------------------------------------
   4. SystemD - INVALID: duplicate field ACROSS systems

   SystemD1 registers cleanly on its own. SystemD2 also looks
   fine in isolation, but it reuses the field name "health" that
   SystemD1 already registered under a totally different
   component/struct. If the registry tracks field names globally
   (not just per-system), registering SystemD2 after SystemD1
   should fail.
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
	eOCT_fieldDescription shieldHealth = { "health", OCT_FIELD_FLOAT, offsetof(testShield, health) };
	eOCT_fieldDescription regenRate = { "regenRate", OCT_FIELD_FLOAT, offsetof(testShield, regenRate) };
	eOCT_fieldDescription shieldFields[] = { shieldHealth, regenRate };

	eOCT_componentDescription shieldComponent = {
		.name = "Shield",
		.stride = sizeof(testShield),
		.providedFields = eOCT_generateFieldDescriptionPool(shieldFields, 2),
	};

	eOCT_componentDescription d1Components[] = { shieldComponent };

	eOCT_systemDescription systemD1 = {
		.name = "SystemD1",
		.providedComponents = eOCT_generateComponentDescriptionPool(d1Components, 1),
		.requestedFields = NULL,
	};

	eOCT_registry_registerSystem(systemD1); /* succeeds */

	/* --- SystemD2: reuses "health", should fail at registration --- */
	eOCT_fieldDescription creatureHealth = { "health", OCT_FIELD_FLOAT, offsetof(testCreatureHealth, health) };
	eOCT_fieldDescription armor = { "armor", OCT_FIELD_FLOAT, offsetof(testCreatureHealth, armor) };
	eOCT_fieldDescription creatureFields[] = { creatureHealth, armor };

	eOCT_componentDescription creatureHealthComponent = {
		.name = "CreatureHealth",
		.stride = sizeof(testCreatureHealth),
		.providedFields = eOCT_generateFieldDescriptionPool(creatureFields, 2),
	};

	eOCT_componentDescription d2Components[] = { creatureHealthComponent };

	eOCT_systemDescription systemD2 = {
		.name = "SystemD2",
		.providedComponents = eOCT_generateComponentDescriptionPool(d2Components, 1),
		.requestedFields = NULL,
	};

	/* EXPECTED: eOCT_registry_registerSystem should reject this,
	   since "health" was already claimed by SystemD1's Shield
	   component. */
	eOCT_registry_registerSystem(systemD2);
}
