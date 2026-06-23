#pragma once
#include "types_eng.h"

#include "utilities/utilities_eng.h"

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

struct eOCT_fieldDescription {
	const char* name;
	eOCT_fieldTypes type;	// standard field types defined in fields.h
	size_t offset;			// offset from the start of the component struct
};

struct eOCT_componentDescription {
	const char* name;
	size_t stride;
	eOCT_fieldDescription* providedFields;
	unsigned int providedFieldsCount;
	eOCT_pool* poolIn; // provided by the system
};

struct eOCT_fieldRequest {
	const char* name;
	eOCT_fieldTypes type;
	bool optional;
};

struct eOCT_systemDescription {
	const char* name;
	eOCT_componentDescription* providedComponents;
	unsigned int providedComponentsCount;
	eOCT_fieldRequest* requestedFields;
	unsigned int requestedFieldsCount;
};

void eOCT_registry_registerSystem(eOCT_systemDescription systemDescription);