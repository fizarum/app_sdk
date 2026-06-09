#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "event.h"
#include "event_type.h"

void broadcaster_init();
void broadcaster_update();

/**
 * @param event is unsigned 32 int which should reflect broadcast_event_t
 */
void broadcaster_send_event(_u32 event);

bool broadcaster_add_listener(const broadcast_event_type_t eventType,
                              const _u16 app_id);
bool broadcaster_remove_listener(const _u16 app_id);

#ifdef __cplusplus
}
#endif
