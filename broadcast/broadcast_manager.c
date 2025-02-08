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

void BroadcastManager_Init() {
  if (manager.initialized == false) {
    manager.initialized = true;
    manager.subs = ArrayCreate(MAX_SUBSCRIBERS_COUNT);
    manager.events = StackCreate(MAX_EVENTS_COUNT);
  }
}

static bool _findSubscriberByEventType(const void* expectedEventType,
                                       const void* subscriber);

BroadcastEvent_t event;

void BroadcastManager_Update() {
  if (ArrayIsEmpty(manager.subs) == true) {
    return;
  }

  if (StackIsEmpty(manager.events) == true) {
    return;
  }

  event.value = (_u32)StackPop(manager.events);

  printf("delivery event type: %lu\n", event.value);

  BroadcastSubsriber_t* subscriber =
      ArrayFind(manager.subs, (void*)(event.type), _findSubscriberByEventType);
  if (subscriber == NULL || subscriber->spec == NULL) {
    return;
  }
  subscriber->spec->onBroadcastEvent(event);
}

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

static bool _findSubscriberByEventType(const void* expectedEventType,
                                       const void* subscriber) {
  return ((BroadcastEventType_t)expectedEventType) ==
         ((BroadcastSubsriber_t*)subscriber)->type;
}

static bool _findSubscriberByAppId(const void* expectedId,
                                   const void* subscriber) {
  return *((_u16*)expectedId) == ((BroadcastSubsriber_t*)subscriber)->spec->id;
}

void BroadcastManager_SendEvent(BroadcastEvent_t event) {
  StackPush(manager.events, (void*)(event.value));
}

void BroadcastManager_SendEventType(BroadcastEventType_t eventType) {
  event.value = 0;
  event.type = eventType;
  StackPush(manager.events, event.value);
}

bool BroadcastManager_AddListener(const BroadcastEventType_t eventType,
                                  const App_t* app) {
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
      ArrayFind(manager.subs, &(spec->id), _findSubscriberByAppId);
  if (subscriber == NULL) {
    return false;
  }

  return ArrayRemove(manager.subs, subscriber);
}