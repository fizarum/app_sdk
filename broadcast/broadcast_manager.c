#include "broadcast_manager.h"

#include <array.h>
#include <map.h>
#include <stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#include "broadcast_event.h"
#include "broadcast_subsriber.h"

#define MAX_SUBSCRIBERS_COUNT 10
#define MAX_EVENTS_COUNT 20

typedef struct broadcast_manager_t {
  bool initialized;
  Array_t* subs;
  Stack_t* events;
} broadcast_manager_t;

static broadcast_manager_t manager = {
    .initialized = false,
    .events = NULL,
    .subs = NULL,
};

broadcast_event_t event = {
    .value = 0,
};

broadcast_subsriber_t* _broadcast_subscriber_create(
    const broadcast_event_type_t type, app_specification_t* spec);

static bool _find_subscriber_by_event_type(const void* expectedEventType,
                                      const void* subscriber);
static bool _find_subscriber_by_app_id(const void* expectedId,
                                  const void* subscriber);

void broadcast_manager_init() {
  if (manager.initialized == false) {
    manager.subs = ArrayCreate(MAX_SUBSCRIBERS_COUNT);
    manager.events = StackCreate(MAX_EVENTS_COUNT);

    manager.initialized = true;
  }
}

void broadcast_manager_update() {
  if (ArrayIsEmpty(manager.subs) == true) {
    return;
  }
  if (StackIsEmpty(manager.events) == true) {
    return;
  }

  event.value = (_u32)StackPop(manager.events);

  broadcast_subsriber_t* subscriber =
      ArrayFind(manager.subs, (void*)(event.type), _find_subscriber_by_event_type);
  if (subscriber == NULL || subscriber->spec == NULL) {
    // event isn't delivered - not such receiver
    printf("event NOT delivered, no receiver, event: %lu\n", event.value);
    return;
  }
  printf("delivery event type: %lu [type: %d, payload: %d]\n", event.value,
         event.type, event.payload);
  subscriber->spec->onBroadcastEvent(event);
}

void broadcast_manager_send_event(_u32 eventArg) {
  if (manager.initialized == false) {
    return;
  }

  StackPush(manager.events, eventArg);
}

bool broadcast_manager_add_listener(const broadcast_event_type_t eventType,
                                    const app_t* app) {
  if (manager.initialized == false) {
    return false;
  }

  if (app == NULL) {
    return false;
  }

  app_specification_t* spec = AppGetSpecification(app);

  printf("subscribed app id [%d] to event type[%d]\n", spec->id, eventType);

  broadcast_subsriber_t* subscriber =
      _broadcast_subscriber_create(eventType, spec);
  if (subscriber == NULL) {
    return false;
  }
  return ArrayAdd(manager.subs, subscriber);
}

bool broadcast_manager_remove_listener(const app_t* app) {
  if (app == NULL) {
    return false;
  }

  app_specification_t* spec = AppGetSpecification(app);

  printf("unsubscribed app id [%d]\n", spec->id);

  broadcast_subsriber_t* subscriber =
      ArrayFind(manager.subs, &(spec->id), _find_subscriber_by_app_id);
  if (subscriber == NULL) {
    return false;
  }

  return ArrayRemove(manager.subs, subscriber);
}

// private part
broadcast_subsriber_t* _broadcast_subscriber_create(
    const broadcast_event_type_t type, app_specification_t* spec) {
  broadcast_subsriber_t* subscriber =
      (broadcast_subsriber_t*)malloc(sizeof(broadcast_subsriber_t));

  if (subscriber == NULL) {
    return NULL;
  }

  subscriber->type = type;
  subscriber->spec = spec;

  return subscriber;
}

static bool _find_subscriber_by_event_type(const void* expectedEventType,
                                      const void* subscriber) {
  return ((broadcast_event_type_t)expectedEventType) ==
         ((broadcast_subsriber_t*)subscriber)->type;
}

static bool _find_subscriber_by_app_id(const void* expectedId,
                                  const void* subscriber) {
  return *((_u16*)expectedId) == ((broadcast_subsriber_t*)subscriber)->spec->id;
}