#ifndef BROADCAST_MANAGER_H
#define BROADCAST_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../app.h"
#include "broadcast_event.h"

typedef struct BroadcastSubsriber_t {
  BroadcastEventType_t type;
  AppSpecification_t* spec;
} BroadcastSubsriber_t;

void BroadcastManager_Init();
void BroadcastManager_Update();

/**
 * @param event is unsigned 32 int which should reflect BroadcastEvent_t
 */
void BroadcastManager_SendEvent(_u32 event);

bool BroadcastManager_AddListener(const BroadcastEventType_t eventType,
                                  const App_t* app);
bool BroadcastManager_RemoveListener(const App_t* spec);

#ifdef __cplusplus
}
#endif

#endif  // BROADCAST_MANAGER_H