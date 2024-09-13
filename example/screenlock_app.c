/**
* example of application build with app sdk
*
*/

#include "screenlock_app.h"
#include <stdio.h>

#define CLOSE_BTN_CODE 120

void (*requestToStop)(const _u16 appId) = NULL;


static AppSpecification_t spec = {
	.name = "Screenlock app",
};

static void handleKey(const void* keyData) {
	_u16 receivedKeyData = *((_u16*)keyData);
	if (receivedKeyData == CLOSE_BTN_CODE) {
		if (requestToStop != NULL) {
			requestToStop(spec.id);
		}
	}
}

static void onAppLoading(void) {
	printf("\t[Screenlock] on app loading...\n");
}

static void onAppUpdate(void) {
	printf("\t[Screenlock] on app update...\n");
	printf("\t press 'X' to unlock device\n");
}

static void onAppStart(void) {
	printf("\t press 'X' to unlock device\n");
}

static void onAppPause(void) {
	printf("\t[Screenlock] on app pause...\n");
}

static void onAppResume(void) {
	printf("\t[Screenlock] on app resume...\n");
}

static void onAppStop(void) {
	printf("\t[Screenlock] on app stop...\n");
}

static void stub(void) {
	printf("\t[Screenlock] stub called\n");
}

AppSpecification_t* ScreenLockAppSpecification(const _u16 id, UserCallback requestToStopCallback) {
	spec.id = id;
	spec.handleInput = &handleKey;
	spec.onInit = &onAppLoading;
	spec.onStart = &onAppStart;
	spec.onPause = &onAppPause;
	spec.onResume = &onAppResume;
	spec.onUpdate = &AppOnUpdate;
	spec.onStop = &onAppStop;

	requestToStop = requestToStopCallback;
	return &spec;
}
