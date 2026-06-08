#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "app.h"

#define APP_ID_NA UINT16_MAX

typedef struct apps_manager_t apps_manager_t;

void apps_manager_init();
app_t* apps_manger_set_launcher(app_specification_t* specs);

/**
 * @brief register application in appsManger.
 * @returns app id if added or APP_ID_NA otherwise
 */
_u16 apps_manager_add(app_specification_t* specs);
void apps_manager_start();
void apps_manager_start_last_added_app();
void apps_manager_start_with_Id(const _u16 appId);

void apps_manager_update();
void apps_manager_handle_input(const void* keyData);
void apps_manager_pause_active_app();
void apps_manager_resume_app();
void apps_manager_stop_app();

Array_t* apps_manager_get_all();
app_t* apps_manager_get_active_app();

#ifdef __cplusplus
}
#endif
