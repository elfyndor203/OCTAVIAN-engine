#pragma once
#include "types_eng.h"

#include "utilities/utilities_eng.h"

enum eOCT_descriptionTypes {
	eOCT_DESC_FIELD,
	eOCT_DESC_COMPONENT,
	eOCT_DESC_SYSTEM,
	eOCT_DESC_REQUEST
};

enum eOCT_fieldTypes {
	eOCT_FIELDTYPE_NULL = 0,
	eOCT_FIELDTYPE_INT64,
	eOCT_FIELDTYPE_UINT64,
	eOCT_FIELDTYPE_FLOAT32,
	eOCT_FIELDTYPE_DOUBLE64,
	eOCT_FIELDTYPE_CHAR8,
	eOCT_FIELDTYPE_PTR64,
	eOCT_FIELDTYPE_STRING64,
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

	OCT_index componentTypeIndex_reg;
};

struct eOCT_componentDescription {
	const char* name;
	size_t stride;
	eOCT_pool providedFields;
	eOCT_componentDescription* cacheLocation;

	OCT_index componentTypeIndex_reg; // where the component is located in the ECS
};

struct eOCT_fieldRequest {
	const char* name;
	eOCT_fieldTypes type;
	//eOCT_fieldAccess access;
	bool optional;

	OCT_index componentTypeIndex_reg;
	size_t fieldOffset_reg;
	bool fulfilled_reg;
};

struct eOCT_systemDescription {
	const char* name;
	eOCT_pool providedComponents;
	eOCT_pool requestedFields;

	OCT_ID systemID_reg; // provided by the registry
};

void eOCT_registry_registerSystem(eOCT_systemDescription* systemDescription);
//void eOCT_registry_allocateComponents(eOCT_componentDescription* componentDescription);
eOCT_pool eOCT_generateFieldDescriptionPool(eOCT_fieldDescription* array, size_t count);
eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, size_t count);
eOCT_pool eOCT_generateFieldRequestPool(eOCT_fieldRequest* array, size_t count);