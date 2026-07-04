#include "OCT_Core_eng.h"
#include <stddef.h>

#include "../internal/systemEx/systemEx_int.h"
#include "../internal/componentEx/componentEx_int.h"

void system_register_systemEx() {

	// FOR EACH COMPONENT
		// Describe each field:
			eOCT_fieldDescription field1 = { "someField", eOCT_FIELDTYPE_INT64, offsetof(iOCT_componentEx, fieldA) };
			eOCT_fieldDescription field2 = { "someOtherField", eOCT_FIELDTYPE_INT64, offsetof(iOCT_componentEx, fieldB) };
		// Create a componentDescription
		eOCT_fieldDescription componentExFields[] = { field1, field2 };
		eOCT_componentDescription componentEx = { "componentEx", sizeof(componentEx), eOCT_generateFieldDescriptionPool(componentExFields, 2)};
	//

	// FOR EACH SYSTEM
		// Collect all components
		eOCT_componentDescription components[] = { componentEx };

		// Describe each field request:
			eOCT_fieldRequest heightReq = { "height", eOCT_FIELDTYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest widthReq = { "width", eOCT_FIELDTYPE_FLOAT32, NULL, true };
			eOCT_fieldRequest mightNeed = { "depth", eOCT_FIELDTYPE_FLOAT32, NULL, true };
		eOCT_fieldRequest requests[] = { heightReq, widthReq, mightNeed };

	//

	// ensure the correct counts
	eOCT_systemDescription templateSystem = {
		.name = "_SystemEx",
		.providedComponents = eOCT_generateComponentDescriptionPool(components, 1),
		.requestedFields = eOCT_generateFieldRequestPool(requests, 3)
	};

	// Store the system somewhere, then register the system from there.
	iOCT_systemEx_inst.system = templateSystem;
	eOCT_registry_registerSystem(&iOCT_systemEx_inst.system);
}