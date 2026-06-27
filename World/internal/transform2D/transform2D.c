#include "transform2D_int.h"
#include "transform2D/transform2D.h"
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "world/world_int.h"

OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 XY) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DCache);
	// __NOTE_
}
OCT_vec2 OCT_transform2D_moveBy(OCT_handle entity, OCT_vec2 deltaXY);


OCT_vec2 iOCT_transform2D_setPosition(iOCT_transform2D* transform, OCT_vec2 xy) {
	transform->position = xy;
	return transform->position;
}