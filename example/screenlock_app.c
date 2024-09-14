/**
* example of application build with app sdk
*
*/

#include "screenlock_app.h"
#include <stdio.h>

static AppSpecification_t spec = {
	.name = "Screenlock app",
};

static void handleKey(const void* keyData) {
	printf("\t[Screenlock] provided keydata: %u\n", *((_u16*)keyData));
}

static void onAppLoading(void) {
	printf("\t[Screenlock] on app loading...\n");
}

static void onAppUpdate(void) {
}

static void onBroadcastEvent(BroadcastEvent_t event) {
	printf("\t[Screenlock] on broadcast event %d\n", event.value);
}

static void onAppStart(void) {
	printf("\t press 'X' to unlock device\n");
}

static void onAppPause(void) {
	printf("\t[Screenlock] on app pause...\n");
}

static void onAppResume(void) {
	printf("\t[Screenlock] on app resume...\n");
	printf("\t press 'X' to unlock device\n");
}

static void onAppStop(void) {
	printf("\t[Screenlock] on app stop...\n");
}

static void stub(void) {
	printf("\t[Screenlock] stub called\n");
}

AppSpecification_t* ScreenLockAppSpecification(const _u16 id) {
	spec.id = id;
	spec.handleInput = &handleKey;
	spec.onInit = &onAppLoading;
	spec.onStart = &onAppStart;
	spec.onPause = &onAppPause;
	spec.onResume = &onAppResume;
	spec.onUpdate = &onAppUpdate;
	spec.onBroadcastEvent = &onBroadcastEvent;
	spec.onStop = &onAppStop;

	return &spec;
}
