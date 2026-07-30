#pragma once

void init_OCT_registry_init();
void init_OCT_ECS_init();
void init_OCT_ECS_build();
void init_OCT_resources_init();
void init_OCT_scheduler_init();
void init_OCT_registry_distributeFields();
void init_OCT_registry_initAllSystems();
void init_OCT_registry_check();

void OCT_engine_tick(); //__NOTE__ bad spot, move to another file