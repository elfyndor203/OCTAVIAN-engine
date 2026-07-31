#pragma once
#include "types_eng.h"
#include "ECS/types_eng.h"

#include "utilities/utilities_eng.h"

typedef enum eOCT_fieldProvider {
	eOCT_FIELDPROVIDER_COMPONENT = 1,
	eOCT_FIELDPROVIDER_EVENT,
	eOCT_FIELDPROVIDER_DATAPOOL,
	eOCT_FIELDPROVIDER_SINGLE
} eOCT_fieldProvider;

typedef enum eOCT_dataTypes { //__NOTE__ typedef all types? or no
	eOCT_DATATYPE_NULL = 0,
	eOCT_DATATYPE_INT64,
	eOCT_DATATYPE_UINT64,
	eOCT_DATATYPE_FLOAT32,
	eOCT_DATATYPE_DOUBLE64,
	eOCT_DATATYPE_CHAR8,
	eOCT_DATATYPE_PTR64,
	eOCT_DATATYPE_STRING64,
	eOCT_DATATYPE_BOOL, // size? include all sizes or just specific?

	eOCT_DATATYPE_ID,
	eOCT_DATATYPE_INDEX,
	eOCT_DATATYPE_VEC2,
	eOCT_DATATYPE_VEC3,
	eOCT_DATATYPE_VEC4,
	eOCT_DATATYPE_MAT3,
	eOCT_DATATYPE_MAT4
} eOCT_dataTypes;

union eOCT_dataUnion {
	int int64;
	uint64_t uint64;
	float float32;
	double double64;
	char char8;
	void* ptr8;
	char* string8;
	OCT_ID ID;
	OCT_index index;
	OCT_vec2 vec2;
	OCT_vec3 vec3;
	OCT_vec4 vec4;
	OCT_mat3 mat3;
};

struct eOCT_fieldTicket {
	const char* name;
	eOCT_dataTypes type;
	size_t offsetFromStruct;
	eOCT_fieldProvider providerType;
	OCT_index providerIndex;

	bool global;
	eOCT_pool* globalPool;
};
struct eOCT_fieldRequest {
	const char* name;
	eOCT_dataTypes type;
	eOCT_fieldTicket* ticketCacheLocation;
	//eOCT_fieldAccess access;
	bool optional;

	eOCT_fieldProvider providerType_reg;
	OCT_index providerIndex_reg;
	size_t fieldOffset_reg;
	bool global_reg;
	bool fulfilled_reg;
};
struct eOCT_fieldDescription {
	const char* name;
	eOCT_dataTypes type;	// standard field types defined in fields.h
	size_t offset;			// offset from the start of the component struct

	eOCT_fieldProvider providerType;
	OCT_index providerIndex_reg;
	bool global_reg;
};
struct eOCT_componentDescription {
	const char* name;
	size_t stride;
	eOCT_pool providedFields;
	eOCT_componentDescription* cacheLocation;
	eOCT_rootAttachmentFx rootAttachmentFx;
	OCT_index sortValueOffset;
	OCT_index entitySlotValueOffset;

	OCT_index componentTypeIndex_reg; // where the component is located in the ECS
};
struct eOCT_dataPoolDescription {
	const char* name;
	size_t stride;
	eOCT_pool providedFields;
	eOCT_dataPoolDescription* cacheLocation;
	bool global;

	OCT_index dataPoolTypeIndex_reg;
};
struct eOCT_eventDescription { // for cross module communication, but what about for the user __NOTE__
	const char* name;
	size_t stride;
	eOCT_pool providedFields;
	eOCT_eventDescription* cacheLocation;
	bool global;

	OCT_index eventTypeIndex_reg;
};
struct eOCT_singleDescription {
	const char* name;
	eOCT_fieldDescription providedField;
	eOCT_singleDescription* cacheLocation;
	bool global;

	OCT_index singleTypeIndex_reg;
};
struct eOCT_systemDescription {
	const char* name;
	eOCT_pool providedComponents;
	eOCT_pool providedDataPools;
	eOCT_pool providedEvents;
	eOCT_pool providedSingles;
	eOCT_pool requestedFields;
	eOCT_systemInitFx initFx;
	// eOCT_systemUpdateFx updateFx;

	OCT_ID systemID_reg; // provided by the registry
};


eOCT_pool* eOCT_field_getSourcePool(OCT_handle contextHandle, eOCT_fieldTicket fieldDetails);
void* eOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex);

void eOCT_registry_registerSystem(eOCT_systemDescription* systemDescription);
//void eOCT_registry_allocateComponents(eOCT_componentDescription* componentDescription);
eOCT_pool eOCT_generateFieldDescriptionPool(eOCT_fieldDescription* array, size_t count);
eOCT_pool eOCT_generateComponentDescriptionPool(eOCT_componentDescription* array, size_t count);
eOCT_pool eOCT_generateDataPoolDescriptionPool(eOCT_dataPoolDescription* array, size_t count);
eOCT_pool eOCT_generateEventDescriptionPool(eOCT_eventDescription* array, size_t count);
eOCT_pool eOCT_generateSingleDescriptionPool(eOCT_singleDescription* array, size_t count);
eOCT_pool eOCT_generateFieldRequestPool(eOCT_fieldRequest* array, size_t count);