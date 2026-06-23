#pragma once
#include "types.h"

#include "utilities/utilities_core.h"

enum cOCT_fieldTypes {
	OCT_FIELD_NULL = 0,
	OCT_FIELD_INT,
	OCT_FIELD_UINT,
	OCT_FIELD_FLOAT,
	OCT_FIELD_DOUBLE,
	OCT_FIELD_CHAR,
	OCT_FIELD_PTR,
	OCT_FIELD_STRING,
};

struct cOCT_fieldDescription {
	const char* name;
	cOCT_fieldTypes type;	// standard field types defined in fields.h
	size_t offset;			// offset from the start of the component struct
};

struct cOCT_componentDescription {
	const char* name;
	size_t stride;
	cOCT_fieldDescription* providedFields;
	unsigned int providedFieldsCount;
};

struct cOCT_fieldRequest {
	const char* name;
	cOCT_fieldTypes type;
	bool optional;
};

struct cOCT_systemDescription {
	const char* name;
	cOCT_componentDescription* providedComponents;
	unsigned int providedComponentsCount;
	cOCT_fieldRequest* requestedFields;
	unsigned int requestedFieldsCount;
};

void cOCT_registry_registerSystem(cOCT_systemDescription systemDescription);