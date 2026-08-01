// /* ============================================================
//    System description variations for the modular ECS registry.
//
//    This version assumes DEFERRED field-request resolution: every
//    system's requestedFields is only checked AFTER all systems in
//    this file have been registered. That means a system can
//    request a field from a component owned by a system that
//    registers later in program order - resolution doesn't care
//    about registration order, only that the field exists exactly
//    once across the whole registry by the time resolution runs.
//
//    Four systems are defined, and this version is fully valid -
//    every requestedFields entry resolves to a real field owned by
//    some system (possibly itself, possibly one registered earlier
//    or later), and no two systems provide the same field name.
//
//    1. testDesc_register  - baseline (Position/Velocity), requests
//                             a field from SystemB even though
//                             SystemB registers after it
//    2. systemB_register   - Rotation/Health, requests a field from
//                             SystemA (registered earlier) and one
//                             from SystemD (registered later)
//    3. systemC_register   - RotationAlt2/RotationAlt3, no more
//                             internal name collision; requests
//                             fields from SystemA and SystemB
//    4. systemD_register   - Shield/CreatureArmor, field names no
//                             longer collide with each other;
//                             requests fields from SystemB and
//                             SystemC
//    ============================================================ */
//
// #include <stddef.h>
// #include "OCT_Core_eng.h"
// #include <stdio.h>
// #include <inttypes.h>
//
// typedef struct systems {
// 	eOCT_systemDescription sysList[5];
// } systems;
//
// systems allSystems = { 0 };
//
// /* ------------------------------------------------------------
//    1. Baseline system - valid
//
//    Requests "rotZ" (provided later by SystemB's Rotation
//    component) and "posB" (its own field - requesting your own
//    provided field is also valid). Both resolve fine once the
//    deferred pass runs after all four systems are registered.
//    ------------------------------------------------------------ */
//
// typedef struct testPos {
// 	float posA;
// 	float posB;
// 	float posC;
// 	float posD;
// 	float posE;
// 	float posF;
// 	float posG;
// } testPos;
//
// typedef struct testVel {
// 	float velA;
// 	float velB;
// } testVel;
//
// void testDesc_register() {
// 	eOCT_fieldDescription posA = { "posA", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posA) };
// 	eOCT_fieldDescription posB = { "posB", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posB) };
// 	eOCT_fieldDescription posC = { "posC", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posC) };
// 	eOCT_fieldDescription posD = { "posD", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posD) };
// 	eOCT_fieldDescription posE = { "posE", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posE) };
// 	eOCT_fieldDescription posF = { "posF", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posF) };
// 	eOCT_fieldDescription posG = { "posG", eOCT_DATATYPE_FLOAT32, offsetof(testPos, posG) };
// 	eOCT_fieldDescription posFields[] = { posA, posB, posC, posD, posE, posF, posG };
//
// 	eOCT_fieldDescription velA = { "velA", eOCT_DATATYPE_FLOAT32, offsetof(testVel, velA) };
// 	eOCT_fieldDescription velB = { "velB", eOCT_DATATYPE_FLOAT32, offsetof(testVel, velB) };
// 	eOCT_fieldDescription velFields[] = { velA, velB };
//
// 	eOCT_componentDescription posComponent = { "Position", sizeof(testPos), eOCT_generateFieldDescriptionPool(7, posA, posB, posC, posD, posE, posF, posG) };
// 	eOCT_componentDescription velComponent = { "Velocity", sizeof(testVel), eOCT_generateFieldDescriptionPool(2, velA, velB) };
// 	eOCT_componentDescription components[] = { posComponent, velComponent };
//
// 	/* "rotZ" doesn't exist yet at the point SystemA registers -
// 	   it's provided later by SystemB. That's fine under deferred
// 	   resolution. "posB" is this system's own field. */
// 	eOCT_fieldRequest rotZ = { "rotZ", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest posBReq = { "posB", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest requests[] = { rotZ, posBReq };
//
// 	eOCT_systemDescription systemA = {
// 		.name = "SystemA",
// 		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
// 		.requestedFields = eOCT_generateFieldRequestPool(requests, 2)
// 	};
//
// 	allSystems.sysList[0] = systemA;
// 	eOCT_registry_registerSystem(&allSystems.sysList[0]);
// }
//
//
// /* ------------------------------------------------------------
//    2. SystemB - valid, different shapes
//
//    Requests "posA" (provided earlier by SystemA) and "shieldRegen"
//    (provided later by SystemD). Both resolve fine in the deferred
//    pass regardless of registration order.
//    ------------------------------------------------------------ */
//
// typedef struct testRot {
// 	float rotX;
// 	float rotY;
// 	float rotW;
// 	float rotZ;
// 	float angVelX;
// 	float angVelY;
// 	float angVelZ;
// 	float angAccelX;
// 	float angAccelY;
// 	float angAccelZ;
// 	float pivotX;
// 	float pivotY;
// 	float pivotZ;
// 	float eulerPitch;
// 	float eulerYaw;
// 	float eulerRoll;
// 	float prevRotX;
// 	float prevRotY;
// 	float prevRotZ;
// 	float prevRotW;
// 	float dampening;
// 	float lockThreshold;
// 	float interpAlpha;
// } testRot;
//
// typedef struct testHealth {
// 	float current;
// 	float max;
// } testHealth;
//
// void systemB_register() {
// 	eOCT_fieldDescription rotX = { "rotX", eOCT_DATATYPE_FLOAT32, offsetof(testRot, rotX) };
// 	eOCT_fieldDescription rotY = { "rotY", eOCT_DATATYPE_FLOAT32, offsetof(testRot, rotY) };
// 	eOCT_fieldDescription rotW = { "rotW", eOCT_DATATYPE_FLOAT32, offsetof(testRot, rotW) };
// 	eOCT_fieldDescription rotZ = { "rotZ", eOCT_DATATYPE_FLOAT32, offsetof(testRot, rotZ) };
// 	eOCT_fieldDescription angVelX = { "angVelX", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angVelX) };
// 	eOCT_fieldDescription angVelY = { "angVelY", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angVelY) };
// 	eOCT_fieldDescription angVelZ = { "angVelZ", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angVelZ) };
// 	eOCT_fieldDescription angAccelX = { "angAccelX", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angAccelX) };
// 	eOCT_fieldDescription angAccelY = { "angAccelY", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angAccelY) };
// 	eOCT_fieldDescription angAccelZ = { "angAccelZ", eOCT_DATATYPE_FLOAT32, offsetof(testRot, angAccelZ) };
// 	eOCT_fieldDescription pivotX = { "pivotX", eOCT_DATATYPE_FLOAT32, offsetof(testRot, pivotX) };
// 	eOCT_fieldDescription pivotY = { "pivotY", eOCT_DATATYPE_FLOAT32, offsetof(testRot, pivotY) };
// 	eOCT_fieldDescription pivotZ = { "pivotZ", eOCT_DATATYPE_FLOAT32, offsetof(testRot, pivotZ) };
// 	eOCT_fieldDescription eulerPitch = { "eulerPitch", eOCT_DATATYPE_FLOAT32, offsetof(testRot, eulerPitch) };
// 	eOCT_fieldDescription eulerYaw = { "eulerYaw", eOCT_DATATYPE_FLOAT32, offsetof(testRot, eulerYaw) };
// 	eOCT_fieldDescription eulerRoll = { "eulerRoll", eOCT_DATATYPE_FLOAT32, offsetof(testRot, eulerRoll) };
// 	eOCT_fieldDescription prevRotX = { "prevRotX", eOCT_DATATYPE_FLOAT32, offsetof(testRot, prevRotX) };
// 	eOCT_fieldDescription prevRotY = { "prevRotY", eOCT_DATATYPE_FLOAT32, offsetof(testRot, prevRotY) };
// 	eOCT_fieldDescription prevRotZ = { "prevRotZ", eOCT_DATATYPE_FLOAT32, offsetof(testRot, prevRotZ) };
// 	eOCT_fieldDescription prevRotW = { "prevRotW", eOCT_DATATYPE_FLOAT32, offsetof(testRot, prevRotW) };
// 	eOCT_fieldDescription dampening = { "dampening", eOCT_DATATYPE_FLOAT32, offsetof(testRot, dampening) };
// 	eOCT_fieldDescription lockThreshold = { "lockThreshold", eOCT_DATATYPE_FLOAT32, offsetof(testRot, lockThreshold) };
// 	eOCT_fieldDescription interpAlpha = { "interpAlpha", eOCT_DATATYPE_FLOAT32, offsetof(testRot, interpAlpha) };
//
// 	eOCT_fieldDescription rotFields[] = {
// 		rotX, rotY, rotW, rotZ,
// 		angVelX, angVelY, angVelZ,
// 		angAccelX, angAccelY, angAccelZ,
// 		pivotX, pivotY, pivotZ,
// 		eulerPitch, eulerYaw, eulerRoll,
// 		prevRotX, prevRotY, prevRotZ, prevRotW,
// 		dampening, lockThreshold, interpAlpha
// 	};
//
// 	eOCT_componentDescription rotComponent = {
// 		.name = "Rotation",
// 		.stride = sizeof(testRot),
// 		.providedFields = eOCT_generateFieldDescriptionPool(23, rotX, rotY, rotW, rotZ,
// 		angVelX, angVelY, angVelZ,
// 		angAccelX, angAccelY, angAccelZ,
// 		pivotX, pivotY, pivotZ,
// 		eulerPitch, eulerYaw, eulerRoll,
// 		prevRotX, prevRotY, prevRotZ, prevRotW,
// 		dampening, lockThreshold, interpAlpha ),
// 	};
//
// 	eOCT_fieldDescription healthCurrent = { "healthCurrent", eOCT_DATATYPE_FLOAT32, offsetof(testHealth, current) };
// 	eOCT_fieldDescription healthMax = { "healthMax", eOCT_DATATYPE_FLOAT32, offsetof(testHealth, max) };
// 	eOCT_fieldDescription healthFields[] = { healthCurrent, healthMax };
//
// 	eOCT_componentDescription healthComponent = {
// 		.name = "Health",
// 		.stride = sizeof(testHealth),
// 		.providedFields = eOCT_generateFieldDescriptionPool(2, healthCurrent, healthMax),
// 	};
//
// 	eOCT_componentDescription components[] = { rotComponent, healthComponent };
//
// 	/* "posA" is provided earlier by SystemA. "shieldRegen" is
// 	   provided later by SystemD - fine under deferred resolution. */
// 	eOCT_fieldRequest reqPosA = { "posA", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest reqShieldRegen = { "shieldRegen", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest requestsB[] = { reqPosA, reqShieldRegen };
//
// 	eOCT_systemDescription systemB = {
// 		.name = "SystemB",
// 		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
// 		.requestedFields = eOCT_generateFieldRequestPool(requestsB, 2),
// 	};
//
// 	allSystems.sysList[1] = systemB;
// 	eOCT_registry_registerSystem(&allSystems.sysList[1]);
// }
//
//
// /* ------------------------------------------------------------
//    3. SystemC - valid
//
//    Originally this system had two components that both provided
//    "rotZ", which is still illegal even under deferred resolution
//    (that's a same-system provided-field collision, not a request
//    resolution question). To make this version fully succeed, the
//    second component's field has been renamed to "rotZAlt" so the
//    two components no longer collide.
//
//    Requests "velA" (provided earlier by SystemA) and "healthMax"
//    (provided earlier by SystemB) - both resolve fine.
//    ------------------------------------------------------------ */
//
// typedef struct testRot2 {
// 	float rotZ;
// 	float rotSpeed;
// } testRot2;
//
// typedef struct testRotAlt {
// 	float rotZAlt;     /* renamed - no longer collides with rotZ above */
// 	float rotDamping;
// } testRotAlt;
//
// void systemC_register() {
// 	eOCT_fieldDescription rotZ_a = { "rotZ_a", eOCT_DATATYPE_FLOAT32, offsetof(testRot2, rotZ) };
// 	eOCT_fieldDescription rotSpeed = { "rotSpeed", eOCT_DATATYPE_FLOAT32, offsetof(testRot2, rotSpeed) };
// 	eOCT_fieldDescription rot2Fields[] = { rotZ_a, rotSpeed };
//
// 	eOCT_componentDescription rotation2Component = {
// 		.name = "Rotation2",
// 		.stride = sizeof(testRot2),
// 		.providedFields = eOCT_generateFieldDescriptionPool(2, rotZ_a, rotSpeed),
// 	};
//
// 	eOCT_fieldDescription rotZAlt_b = { "rotZAlt", eOCT_DATATYPE_FLOAT32, offsetof(testRotAlt, rotZAlt) };
// 	eOCT_fieldDescription rotDamping = { "rotDamping", eOCT_DATATYPE_FLOAT32, offsetof(testRotAlt, rotDamping) };
// 	eOCT_fieldDescription rotAltFields[] = { rotZAlt_b, rotDamping };
//
// 	eOCT_componentDescription rotationAltComponent = {
// 		.name = "RotationAlt",
// 		.stride = sizeof(testRotAlt),
// 		.providedFields = eOCT_generateFieldDescriptionPool(2, rotZAlt_b, rotAltFields),
// 	};
//
// 	eOCT_componentDescription components[] = { rotation2Component, rotationAltComponent };
//
// 	/* "velA" provided earlier by SystemA, "healthMax" provided
// 	   earlier by SystemB. Both resolve fine. */
// 	eOCT_fieldRequest reqVelA = { "velA", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest reqHealthMax = { "healthMax", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest requestsC[] = { reqVelA, reqHealthMax };
//
// 	eOCT_systemDescription systemC = {
// 		.name = "SystemC",
// 		.providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
// 		.requestedFields = eOCT_generateFieldRequestPool(requestsC, 2),
// 	};
//
// 	allSystems.sysList[2] = systemC;
// 	eOCT_registry_registerSystem(&allSystems.sysList[2]);
// }
//
//
// /* ------------------------------------------------------------
//    4. SystemD - valid
//
//    Originally SystemD1 and SystemD2 both provided a field named
//    "health", which is illegal under any resolution model (it's a
//    cross-system provided-field name collision, found during the
//    deferred pass once it scans every provided field across every
//    registered system). To make this version fully succeed,
//    SystemD2's field has been renamed to "creatureHealth" so the
//    two no longer collide.
//
//    SystemD1 requests "rotSpeed" (provided earlier by SystemC).
//    SystemD2 requests "creatureHealth" (its sibling system's own
//    field, still registered earlier in this same function) and
//    "posC" (provided earlier by SystemA). All four resolve fine.
//    ------------------------------------------------------------ */
//
// typedef struct testShield {
// 	float shieldHealth;   /* renamed - no longer collides with creatureHealth below */
// 	float shieldRegen;    /* this is the field SystemB requests from "later" */
// } testShield;
//
// typedef struct testCreatureHealth {
// 	float creatureHealth; /* renamed - no longer collides with shieldHealth above */
// 	float armor;
// } testCreatureHealth;
//
// void systemD_register() {
// 	/* --- SystemD1 --- */
// 	eOCT_fieldDescription shieldHealth = { "shieldHealth", eOCT_DATATYPE_FLOAT32, offsetof(testShield, shieldHealth) };
// 	eOCT_fieldDescription shieldRegen = { "shieldRegen", eOCT_DATATYPE_FLOAT32, offsetof(testShield, shieldRegen) };
// 	eOCT_fieldDescription shieldFields[] = { shieldHealth, shieldRegen };
//
// 	eOCT_componentDescription shieldComponent = {
// 		.name = "Shield",
// 		.stride = sizeof(testShield),
// 		.providedFields = eOCT_generateFieldDescriptionPool(2,shieldHealth, shieldRegen),
// 	};
//
// 	eOCT_componentDescription d1Components[] = { shieldComponent };
//
// 	/* "rotSpeed" provided earlier by SystemC. */
// 	eOCT_fieldRequest reqRotSpeed = { "rotSpeed", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest requestsD1[] = { reqRotSpeed };
//
// 	eOCT_systemDescription systemD1 = {
// 		.name = "SystemD1",
// 		.providedComponents = eOCT_generateComponentDescriptionPool(d1Components, 1),
// 		.requestedFields = eOCT_generateFieldRequestPool(requestsD1, 1),
// 	};
//
// 	/* --- SystemD2 --- */
// 	eOCT_fieldDescription creatureHealth = { "creatureHealth", eOCT_DATATYPE_FLOAT32, offsetof(testCreatureHealth, creatureHealth) };
// 	eOCT_fieldDescription armor = { "armor", eOCT_DATATYPE_FLOAT32, offsetof(testCreatureHealth, armor) };
// 	eOCT_fieldDescription creatureFields[] = { creatureHealth, armor };
//
// 	eOCT_componentDescription creatureHealthComponent = {
// 		.name = "CreatureHealth",
// 		.stride = sizeof(testCreatureHealth),
// 		.providedFields = eOCT_generateFieldDescriptionPool(2, creatureHealth, armor),
// 	};
//
// 	eOCT_componentDescription d2Components[] = { creatureHealthComponent };
//
// 	/* "creatureHealth" is SystemD2's own sibling field (provided
// 	   by SystemD1 in this same call, registered just above), and
// 	   "posC" is provided earlier by SystemA. Both resolve fine. */
// 	eOCT_fieldRequest reqCreatureHealth = { "creatureHealth", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest reqPosC = { "posC", eOCT_DATATYPE_FLOAT32 };
// 	eOCT_fieldRequest requestsD2[] = { reqCreatureHealth, reqPosC };
//
// 	eOCT_systemDescription systemD2 = {
// 		.name = "SystemD2",
// 		.providedComponents = eOCT_generateComponentDescriptionPool(d2Components, 1),
// 		.requestedFields = eOCT_generateFieldRequestPool(requestsD2, 2),
// 	};
//
// 	allSystems.sysList[3] = systemD1;
// 	allSystems.sysList[4] = systemD2;
// 	eOCT_registry_registerSystem(&allSystems.sysList[3]);
// 	eOCT_registry_registerSystem(&allSystems.sysList[4]);
// }
