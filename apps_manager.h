#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "app.h"

// #define APP_ID_NA UINT16_MAX

typedef struct apps_manager_t apps_manager_t;

void apps_manager_init();
_u16 apps_manger_set_launcher(app_specification_t* specs);
void apps_manager_start();
void apps_manager_update();
void apps_manager_handle_input(const void* key_data);

/**
 * @brief register application in appsManger.
 * @returns app id if added or APP_ID_NA otherwise
 */
_u16 apps_manager_register(app_specification_t* specs);

bool apps_manager_open_app(const _u16 app_id);
bool apps_manager_close_app(const _u16 app_id);

Array_t* apps_manager_get_all();
Array_t* apps_manager_get_running_apps();

app_t* apps_manager_get_app(const _u16 app_id);

#ifdef __cplusplus
}
#endif
