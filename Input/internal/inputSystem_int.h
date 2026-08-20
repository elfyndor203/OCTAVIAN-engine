#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_inputSystem {
    OCT_ID systemID;

    eOCT_fieldTicket keyTicket;
    eOCT_fieldTicket keyPressTicket;
    eOCT_fieldTicket keyReleaseTicket;
    eOCT_fieldTicket mouseButtonTicket;
    eOCT_fieldTicket mouseButtonPressTicket;
    eOCT_fieldTicket mouseButtonReleaseTicket;
    eOCT_fieldTicket mousePositionXTicket;
    eOCT_fieldTicket mousePositionYTicket;
    eOCT_fieldTicket mouseScrollTicket;
    eOCT_fieldTicket cameraMatrixTicket;
    eOCT_fieldTicket deltaTimeTicket;
    eOCT_componentKey interactBoxKey;

    OCT_vec2 mousePosition;
    float mouseScrollDelta;

    eOCT_pool buttonStates;
};

extern iOCT_inputSystem iOCT_inputSystem_inst;

void system_init_INPUT();