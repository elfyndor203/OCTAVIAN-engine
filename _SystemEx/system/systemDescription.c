#include "OCT_Core_eng.h"
#include <stddef.h>

#include "systemEx/systemEx_int.h"
#include "componentEx/componentEx_int.h"

void system_register_systemEx() {

	// FOR EACH COMPONENT
		// Describe each field:
			eOCT_fieldDescription field1 = { "someField", eOCT_DATATYPE_INT64, offsetof(iOCT_componentEx, fieldA) };
			eOCT_fieldDescription field2 = { "someOtherField", eOCT_DATATYPE_INT64, offsetof(iOCT_componentEx, fieldB) };
		// Create a componentDescription
		eOCT_componentDescription componentEx = { "componentEx", sizeof(componentEx), eOCT_generateFieldDescriptionPool(2, field1, field2)};
	//

	// FOR EACH SYSTEM
		// Describe each field request:
			eOCT_fieldRequest heightReq = { "height", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest widthReq = { "width", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest mightNeed = { "depth", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest keys = {
				.name = "glfwKeys",
				.type = eOCT_DATATYPE_INT64,
				.optional = false,
				.ticketCache_ifLocal = &iOCT_systemEx_inst.keyCache
			};
			eOCT_fieldRequest keyPress = {
				.name = "glfwKeyPress",
				.type = eOCT_DATATYPE_BOOL,
				.optional = false,
				.ticketCache_ifLocal = &iOCT_systemEx_inst.keyPressCache
			};
			eOCT_fieldRequest keyRelease = {
				.name = "glfwKeyRelease",
				.type = eOCT_DATATYPE_BOOL,
				.optional = false,
				.ticketCache_ifLocal = &iOCT_systemEx_inst.keyReleaseCache
			};
	//

	eOCT_fieldDescription testSingleField = {
		.name = "testSingleField",
		.type = eOCT_DATATYPE_INT64,
		.providerType = eOCT_DATAPATTERN_SINGLE,
		.offset = 0
	};
	eOCT_singleDescription testSingle = {
		.name = "testGlobal",
		.providedField = testSingleField,
		.keyCacheLocation = NULL,
		.global = true
	};

	// ensure the correct counts
	eOCT_systemDescription templateSystem = {
		.name = "_SystemEx",
		.providedComponents = eOCT_generateComponentDescriptionPool(1, componentEx),
		.requestedFields = eOCT_generateFieldRequestPool(6, heightReq, widthReq, mightNeed, keys, keyPress, keyRelease),
		.providedEvents = eOCT_POOL_EMPTY,
		.providedSingles = eOCT_generateSingleDescriptionPool(1, testSingle),
		.providedDataPools = eOCT_POOL_EMPTY,
		.initFx = system_init_systemEx
	};

	iOCT_systemEx_inst.systemID = eOCT_registry_registerSystem(templateSystem);
}