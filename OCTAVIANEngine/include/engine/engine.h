#pragma once

void OCT_engine_init();
// void OCT_engine_start(char* name, unsigned int windowX, unsigned int windowY, OCT_vec4 BGColor, unsigned int virtualX, unsigned int virtualY, unsigned int maxFPS, unsigned int ECSHz, unsigned int physicsHz);
// void OCT_engine_stop();
void OCT_engine_update(OCT_handle context);