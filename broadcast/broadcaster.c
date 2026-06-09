#include "broadcaster.h"

#include <array.h>
#include <map.h>
#include <stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#include "../apps_manager.h"
#include "event.h"
#include "subscriber.h"

#define MAX_SUBSCRIBERS_COUNT 10
#define MAX_EVENTS_COUNT 20

typedef struct broadcaster_t {
  bool initialized;
  Array_t* subs;
  Stack_t* events;
} broadcaster_t;

static broadcaster_t manager = {
    .initialized = false,
    .events = NULL,
    .subs = NULL,
};

broadcast_event_t event = {
    .value = 0,
};

broadcast_subsriber_t* _create_subscriber(const broadcast_event_type_t type,
                                          _u16 id);
static bool _find_subscriber_by_event_type(const void* expectedEventType,
                                           const void* subscriber);
static bool _find_subscriber_by_app_id(const void* expectedId,
                                       const void* subscriber);

void broadcaster_init() {
  if (manager.initialized == false) {
    manager.subs = ArrayCreate(MAX_SUBSCRIBERS_COUNT);
    manager.events = StackCreate(MAX_EVENTS_COUNT);

    manager.initialized = true;
  }
}

void broadcaster_update() {
  if (ArrayIsEmpty(manager.subs) == true) {
    return;
  }
  if (StackIsEmpty(manager.events) == true) {
    return;
  }

  event.value = (_u32)StackPop(manager.events);

  broadcast_subsriber_t* subscriber = ArrayFind(
      manager.subs, (void*)(event.type), _find_subscriber_by_event_type);

  if (subscriber == NULL || subscriber->subscriber_id == APP_ID_NA) {
    // event isn't delivered - not such receiver
    printf("event NOT delivered, no receiver, event: %lu\n", event.value);
    return;
  }
  printf("delivery event type: %lu [type: %d, payload: %d]\n", event.value,
         event.type, event.payload);
  app_t* app = apps_manager_get_app(subscriber->subscriber_id);
  app_specification_t* specs = AppGetSpecification(app);

  printf("app id: %lu\n", subscriber->subscriber_id);

  if (specs != NULL) {
    specs->onBroadcastEvent(event);
  }
}

void broadcaster_send_event(_u32 eventArg) {
  if (manager.initialized == false) {
    return;
  }

  StackPush(manager.events, eventArg);
}

bool broadcaster_add_listener(const broadcast_event_type_t eventType,
                              const _u16 app_id) {
  if (manager.initialized == false) {
    return false;
  }

  printf("subscribed app id [%d] to event type[%d]\n", app_id, eventType);

  broadcast_subsriber_t* subscriber = _create_subscriber(eventType, app_id);
  if (subscriber == NULL) {
    return false;
  }
  return ArrayAdd(manager.subs, subscriber);
}

bool broadcaster_remove_listener(const _u16 app_id) {
  app_t* app = apps_manager_get_app(app_id);
  app_specification_t* spec = AppGetSpecification(app);
  if (spec == NULL) {
    return false;
  }

  printf("unsubscribed app id [%d]\n", spec->id);

  broadcast_subsriber_t* subscriber =
      ArrayFind(manager.subs, &(spec->id), _find_subscriber_by_app_id);
  if (subscriber == NULL) {
    return false;
  }

  return ArrayRemove(manager.subs, subscriber);
}

// private part
broadcast_subsriber_t* _create_subscriber(const broadcast_event_type_t type,
                                          _u16 id) {
  broadcast_subsriber_t* subscriber =
      (broadcast_subsriber_t*)malloc(sizeof(broadcast_subsriber_t));

  if (subscriber == NULL) {
    return NULL;
  }

  subscriber->type = type;
  subscriber->subscriber_id = id;

  return subscriber;
}

static bool _find_subscriber_by_event_type(const void* expectedEventType,
                                           const void* subscriber) {
  return ((broadcast_event_type_t)expectedEventType) ==
         ((broadcast_subsriber_t*)subscriber)->type;
}

static bool _find_subscriber_by_app_id(const void* expectedId,
                                       const void* subscriber) {
  broadcast_subsriber_t* subs = (broadcast_subsriber_t*)subscriber;

  return *((_u16*)expectedId) == subs->subscriber_id;
}