// #include "scheduler_int.h"
// #include "types_int.h"
//
// #include "layout/systems.h"
// #include "registry/types_eng.h"
// #include "ECS/ECS_int.h"
// #include "ECS/entityContext_int.h"
//
// iOCT_scheduler iOCT_scheduler_inst = { 0 };
//
// void init_OCT_scheduler_init() {
//     iOCT_scheduler_inst.updateFxPool = eOCT_pool_init(OCT_ID_SCHEDULER, eOCT_POOL_SIZE_DEFAULT, sizeof(eOCT_systemUpdateFx));
// }
//
// void iOCT_scheduler_addUpdateFx(eOCT_systemUpdateFx updateFx) {
//     eOCT_systemUpdateFx* function = (eOCT_systemUpdateFx*)eOCT_pool_addEntry(&iOCT_scheduler_inst.updateFxPool, NULL);
//     *function = updateFx;
// }
//
// void iOCT_scheduler_tick() {
//     eOCT_pool* contextPool = &iOCT_ECS_inst.contextPool;
//     iOCT_entityContext* contextArray = (iOCT_entityContext*)contextPool->array;
//
//     eOCT_pool* updateFxPool = &iOCT_scheduler_inst.updateFxPool;
//     eOCT_systemUpdateFx* updateFxArray = (eOCT_systemUpdateFx*)updateFxPool->array;
//
//     // for each context
//     OCT_handle contextHandle;
//     for (OCT_index contextCtr = 0; contextCtr < contextPool->count; contextCtr++) {
//         iOCT_entityContext* context = &contextArray[contextCtr];
//         contextHandle = iOCT_getContextHandle(context);
//
//         // try each update function
//         for (OCT_index updateFxCtr = 0; updateFxCtr < updateFxPool->count; updateFxCtr++) {
//             eOCT_systemUpdateFx updateFx = updateFxArray[updateFxCtr];
//             if (updateFx) {
//                 updateFx(contextHandle);
//             }
//         }
//     }
// }
//
// void OCT_engine_tick() {
//     iOCT_scheduler_tick();
// }
