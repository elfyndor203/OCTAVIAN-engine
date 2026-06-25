#include "OCT_Core_eng.h"
#include "registry/registry_int.h"
#include "ECS/entity/entityContext_int.h"
#include "testSystem.h"

typedef struct testPos {
	float posA;
	float posB;
} testPos;

typedef struct testVel {
	float velA;
	float velB;

} testVel;;



int main() {
	OCT_engine_init();
	iOCT_entityContext_open();
	return 0;
}