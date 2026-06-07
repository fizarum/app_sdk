#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../app.h"
#include "broadcast_event.h"

void broadcast_manager_init();
void broadcast_manager_update();

/**
 * @param event is unsigned 32 int which should reflect broadcast_event_t
 */
void broadcast_manager_send_event(_u32 event);

bool broadcast_manager_add_listener(const broadcast_event_type_t eventType,
                                    const app_t* app);
bool broadcast_manager_remove_listener(const app_t* spec);

#ifdef __cplusplus
}
#endif
