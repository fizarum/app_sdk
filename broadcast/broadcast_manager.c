#include "broadcast_manager.h"

#include <array.h>
#include <map.h>
#include <stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#include "broadcast_event.h"

const _u8 MAX_SUBSCRIBERS_COUNT = 10;
const _u8 MAX_EVENTS_COUNT = 20;

typedef struct BroadcastManager_t {
  bool initialized;
  Array_t* subs;
  Stack_t* events;

} BroadcastManager_t;

static BroadcastManager_t manager = {
    .initialized = false,
    .events = NULL,
    .subs = NULL,
};

BroadcastSubsriber_t* BroadcastSubsriber_Create(const BroadcastEventType_t type,
                                                AppSpecification_t* spec);

static bool FindSubscriberByEventType(const void* expectedEventType,
                                      const void* subscriber);
static bool FindSubscriberByAppId(const void* expectedId,
                                  const void* subscriber);

BroadcastEvent_t event;

void BroadcastManager_Init() {
  if (manager.initialized == false) {
    manager.subs = ArrayCreate(MAX_SUBSCRIBERS_COUNT);
    manager.events = StackCreate(MAX_EVENTS_COUNT);

    manager.initialized = true;
  }
}

void BroadcastManager_Update() {
  if (ArrayIsEmpty(manager.subs) == true) {
    return;
  }

  if (StackIsEmpty(manager.events) == true) {
    return;
  }

  event.value = (_u32)StackPop(manager.events);

  BroadcastSubsriber_t* subscriber =
      ArrayFind(manager.subs, (void*)(event.type), FindSubscriberByEventType);
  if (subscriber == NULL || subscriber->spec == NULL) {
    // event isn't delivered - not such receiver
    printf("event NOT delivered, no receiver, event: %lu\n", event.value);
    return;
  }
  printf("delivery event type: %lu\n", event.value);
  subscriber->spec->onBroadcastEvent(event);
}

void BroadcastManager_SendEvent(_u32 eventArg) {
  if (manager.initialized == false) {
    return;
  }

  StackPush(manager.events, eventArg);
}

bool BroadcastManager_AddListener(const BroadcastEventType_t eventType,
                                  const App_t* app) {
  if (manager.initialized == false) {
    return false;
  }

  if (app == NULL) {
    return false;
  }

  AppSpecification_t* spec = AppGetSpecification(app);

  printf("subscribed app id [%d] to event type[%d]\n", spec->id, eventType);

  BroadcastSubsriber_t* subscriber = BroadcastSubsriber_Create(eventType, spec);
  if (subscriber == NULL) {
    return false;
  }
  return ArrayAdd(manager.subs, subscriber);
}

bool BroadcastManager_RemoveListener(const App_t* app) {
  if (app == NULL) {
    return false;
  }

  AppSpecification_t* spec = AppGetSpecification(app);

  printf("unsubscribed app id [%d]\n", spec->id);

  BroadcastSubsriber_t* subscriber =
      ArrayFind(manager.subs, &(spec->id), FindSubscriberByAppId);
  if (subscriber == NULL) {
    return false;
  }

  return ArrayRemove(manager.subs, subscriber);
}

// private part
BroadcastSubsriber_t* BroadcastSubsriber_Create(const BroadcastEventType_t type,
                                                AppSpecification_t* spec) {
  BroadcastSubsriber_t* subscriber =
      (BroadcastSubsriber_t*)malloc(sizeof(BroadcastSubsriber_t));

  if (subscriber == NULL) {
    return NULL;
  }

  subscriber->type = type;
  subscriber->spec = spec;

  return subscriber;
}

static bool FindSubscriberByEventType(const void* expectedEventType,
                                      const void* subscriber) {
  return ((BroadcastEventType_t)expectedEventType) ==
         ((BroadcastSubsriber_t*)subscriber)->type;
}

static bool FindSubscriberByAppId(const void* expectedId,
                                  const void* subscriber) {
  return *((_u16*)expectedId) == ((BroadcastSubsriber_t*)subscriber)->spec->id;
}