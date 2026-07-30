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
		eOCT_fieldDescription componentExFields[] = { field1, field2 };
		eOCT_componentDescription componentEx = { "componentEx", sizeof(componentEx), eOCT_generateFieldDescriptionPool(componentExFields, 2)};
	//

	// FOR EACH SYSTEM
		// Collect all components
		eOCT_componentDescription components[] = { componentEx };

		// Describe each field request:
			eOCT_fieldRequest heightReq = { "height", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest widthReq = { "width", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest mightNeed = { "depth", eOCT_DATATYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest keys = {
				.name = "glfwKeys",
				.type = eOCT_DATATYPE_INT64,
				.optional = false,
				.cacheLocation = &iOCT_systemEx_inst.keyCache
			};
			eOCT_fieldRequest keyPress = {
				.name = "glfwKeyPress",
				.type = eOCT_DATATYPE_BOOL,
				.optional = false,
				.cacheLocation = &iOCT_systemEx_inst.keyPressCache
			};
			eOCT_fieldRequest keyRelease = {
				.name = "glfwKeyRelease",
				.type = eOCT_DATATYPE_BOOL,
				.optional = false,
				.cacheLocation = &iOCT_systemEx_inst.keyReleaseCache
			};
		eOCT_fieldRequest requests[6] = { heightReq, widthReq, mightNeed, keys, keyPress, keyRelease };

	//

	// ensure the correct counts
	eOCT_systemDescription templateSystem = {
		.name = "_SystemEx",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 1),
		.requestedFields = eOCT_generateFieldRequestPool(requests, 6),
		.providedEvents = eOCT_POOL_EMPTY,
		.providedDataPools = eOCT_POOL_EMPTY,
		.initFx = system_init_systemEx
	};

	// Store the system somewhere, then register the system from there.
	iOCT_systemEx_inst.system = templateSystem;
	eOCT_registry_registerSystem(&iOCT_systemEx_inst.system);
}