#pragma once
#include "types_eng.h"

#include "utilities/utilities_eng.h"


enum eOCT_descriptionTypes {
	eOCT_DESC_FIELD,
	eOCT_DESC_COMPONENT,
	eOCT_DESC_SYSTEM
};

enum eOCT_fieldTypes {
	OCT_FIELD_NULL = 0,
	OCT_FIELD_INT,
	OCT_FIELD_UINT,
	OCT_FIELD_FLOAT,
	OCT_FIELD_DOUBLE,
	OCT_FIELD_CHAR,
	OCT_FIELD_PTR,
	OCT_FIELD_STRING,
};

enum eOCT_fieldAccess {
	OCT_FIELD_ACCESS_NONE = 0,	// 00
	OCT_FIELD_ACCESS_READ = 1 << 0,	 // 01
	OCT_FIELD_ACCESS_WRITE = 1 << 1, // 10
	OCT_FIELD_ACCESS_READWRITE = OCT_FIELD_ACCESS_READ | OCT_FIELD_ACCESS_WRITE, // 11
};

struct eOCT_fieldDescription {
	const char* name;
	eOCT_fieldTypes type;	// standard field types defined in fields.h
	size_t offset;			// offset from the start of the component struct
};

struct eOCT_componentDescription {
	const char* name;
	size_t stride;
	eOCT_pool providedFields;

	OCT_ID componentIndex_reg; // where the component is located in the ECS
	eOCT_pool* poolIn_reg; // provided by the registry, where the system can access its own components
};

struct eOCT_fieldRequest {
	const char* name;
	eOCT_fieldTypes type;
	bool optional;

	eOCT_pool* readPool_reg; // provided by the registry, systems can read and write to individual fields
	size_t readOffset_reg;
	eOCT_pool* writePool_reg;
	size_t writeOffset_reg;
};

struct eOCT_systemDescription {
	OCT_ID systemID_reg; // provided by the registry

	const char* name;
	eOCT_pool providedComponents;
	eOCT_pool requestedFields;
};

void eOCT_registry_registerSystem(eOCT_systemDescription systemDescription);
//void eOCT_registry_allocateComponents(eOCT_componentDescription* componentDescription);
eOCT_pool eOCT_generateFieldDescriptionPool(eOCT_fieldDescription* array, unsigned int count);
eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, unsigned int count);