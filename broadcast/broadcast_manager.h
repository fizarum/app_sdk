#ifndef BROADCAST_MANAGER_H
#define BROADCAST_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../app.h"
#include "broadcast_event.h"

typedef struct BroadcastManager_t BroadcastManager_t;

typedef struct BroadcastSubsriber_t {
  BroadcastEventType_t type;
  AppSpecification_t* spec;
} BroadcastSubsriber_t;

BroadcastManager_t* BroadcastManagerCreate();
BroadcastSubsriber_t* BroadcastSubsriberCreate(const BroadcastEventType_t type,
                                               AppSpecification_t* spec);

void BroadcastManagerUpdate(BroadcastManager_t* manager);
void BroadcastManagerSendEvent(BroadcastManager_t* manager,
                               BroadcastEvent_t event);

bool BroadcastManagerSubscribe(const BroadcastManager_t* manager,
                               const BroadcastEventType_t eventType,
                               const App_t* app);
bool BroadcastManagerUnsubscribe(const BroadcastManager_t* manager,
                                 const App_t* spec);

#ifdef __cplusplus
}
#endif

#endif  // BROADCAST_MANAGER_H