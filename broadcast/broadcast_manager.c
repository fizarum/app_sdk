#include "broadcast_manager.h"
#include "broadcast_event.h"

#include <types.h>
#include <map.h>
#include <array.h>
#include <stack.h>
#include <stdlib.h>


const _u8 MAX_SUBSCRIBERS_COUNT = 10;
const _u8 MAX_EVENTS_COUNT = 20;

typedef struct BroadcastManager_t {
	Array_t* subs;
	Stack_t* events;

} BroadcastManager_t;

BroadcastManager_t* BroadcastManagerCreate() {
	BroadcastManager_t* manager =
		(BroadcastManager_t*)malloc(sizeof(BroadcastManager_t));

	if (manager == NULL) {
		return NULL;
	}
	manager->subs = ArrayCreate(MAX_SUBSCRIBERS_COUNT);
	manager->events = StackCreate(MAX_EVENTS_COUNT);

	return manager;
}

BroadcastSubsriber_t* BroadcastSubsriberCreate(const BroadcastEventType_t type,
	const AppSpecification_t* spec) {
	BroadcastSubsriber_t* subscriber = (BroadcastSubsriber_t*)malloc(sizeof(BroadcastSubsriber_t));

	if (subscriber == NULL) {
		return NULL;
	}

	subscriber->type = type;
	subscriber->spec = spec;

	return subscriber;
}

#include <stdio.h>

BroadcastEvent_t event;

bool _findSubscriberByEventType(const void* expectedEventType, const BroadcastSubsriber_t* subscriber) {
	return (BroadcastEventType_t)expectedEventType == subscriber->type;
}

bool _findSubscriberByAppId(const void* expectedId, const BroadcastSubsriber_t* subscriber) {
	return (_u16)expectedId == subscriber->spec->id;
}

void BroadcastManagerUpdate(BroadcastManager_t* manager) {
	if (ArrayIsEmpty(manager->subs) == true) {
		return;
	}

	if (StackIsEmpty(manager->events) == true) {
		return;
	}

	event.value = (_u32)StackPop(manager->events);

	printf("delivery event type: %d\n", event.value);
	BroadcastSubsriber_t* subscriber = ArrayFind(manager->subs, event.type, _findSubscriberByEventType);
	if (subscriber == NULL) {
		return;
	}
	subscriber->spec->onBroadcastEvent(event);
}

void BroadcastManagerSendEvent(BroadcastManager_t* manager, BroadcastEvent_t event) {
	StackPush(manager->events, (void*)(event.value));
}

bool BroadcastManagerSubscribe(const BroadcastManager_t* manager, const BroadcastEventType_t eventType, App_t* app) {

	if (app == NULL) {
		return false;
	}

	AppSpecification_t* spec = AppGetSpecification(app);

	printf("subscribed app id [%d] to event type[%d]\n", spec->id, eventType);

	BroadcastSubsriber_t* subscriber = BroadcastSubsriberCreate(eventType, spec);
	if (subscriber == NULL) {
		return false;
	}
	return ArrayAdd(manager->subs, subscriber);
}

bool BroadcastManagerUnsubscribe(const BroadcastManager_t* manager, const App_t* app) {

	if (app == NULL) {
		return false;
	}

	AppSpecification_t* spec = AppGetSpecification(app);

	printf("unsubscribed app id [%d]\n", spec->id);

	BroadcastSubsriber_t* found = ArrayFind(manager->subs, spec->id, _findSubscriberByAppId);
	if (found == NULL) {
		return false;
	}

	return ArrayRemove(manager->subs, found);
}